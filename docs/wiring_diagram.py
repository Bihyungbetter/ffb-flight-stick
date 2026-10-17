"""Generates docs/wiring.svg. Run: python docs/wiring_diagram.py"""
import math
from pathlib import Path

W, H = 1350, 790
FONT = "Helvetica, Arial, sans-serif"

C = {
    "v12": "#d62728",
    "gnd": "#222222",
    "v33": "#e67e22",
    "pwm": "#1f77b4",
    "en": "#9467bd",
    "sck": "#2ca02c",
    "miso": "#17a2b8",
    "mosi": "#b8860b",
    "cs": "#e377c2",
    "adc": "#8c564b",
    "phase": "#6b6b6b",
    "usb": "#555555",
    "box": "#333333",
}

out = []
wires = []  # (net, points, color, width)


def text(x, y, s, size=12, anchor="start", weight="normal", color="#222", italic=False):
    style = ' font-style="italic"' if italic else ""
    out.append(
        f'<text x="{x}" y="{y}" font-size="{size}" text-anchor="{anchor}" '
        f'font-weight="{weight}" fill="{color}"{style}>{s}</text>'
    )


def box(x, y, w, h, fill="#ffffff", stroke=C["box"], rx=3, dash=None):
    d = f' stroke-dasharray="{dash}"' if dash else ""
    out.append(
        f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="{rx}" '
        f'fill="{fill}" stroke="{stroke}" stroke-width="2"{d}/>'
    )


def wire(net, pts, color, width=2.5):
    wires.append((net, pts, color, width))


def dot(x, y, color):
    out.append(f'<circle cx="{x}" cy="{y}" r="4" fill="{color}"/>')


def pin(x, y, color):
    out.append(f'<circle cx="{x}" cy="{y}" r="3.5" fill="#fff" stroke="{color}" stroke-width="2"/>')


def flag_3v3(x, y):
    """Power flag: stub left from (x, y) to a 3V3 label."""
    out.append(f'<line x1="{x}" y1="{y}" x2="{x-22}" y2="{y}" stroke="{C["v33"]}" stroke-width="2.5"/>')
    out.append(f'<line x1="{x-22}" y1="{y-7}" x2="{x-22}" y2="{y+7}" stroke="{C["v33"]}" stroke-width="2.5"/>')
    text(x - 27, y + 4, "3V3", 11, "end", "bold", C["v33"])


def flag_gnd(x, y):
    """Ground symbol on a stub left from (x, y)."""
    g = C["gnd"]
    out.append(f'<line x1="{x}" y1="{y}" x2="{x-22}" y2="{y}" stroke="{g}" stroke-width="2.5"/>')
    for i, half in enumerate((8, 5, 2)):
        xx = x - 22 - i * 4
        out.append(f'<line x1="{xx}" y1="{y-half}" x2="{xx}" y2="{y+half}" stroke="{g}" stroke-width="2"/>')
    text(x - 36, y + 4, "GND", 11, "end", "bold", g)


def flag_3v3_right(x, y):
    out.append(f'<line x1="{x}" y1="{y}" x2="{x+22}" y2="{y}" stroke="{C["v33"]}" stroke-width="2.5"/>')
    out.append(f'<line x1="{x+22}" y1="{y-7}" x2="{x+22}" y2="{y+7}" stroke="{C["v33"]}" stroke-width="2.5"/>')
    text(x + 27, y + 4, "3V3", 11, "start", "bold", C["v33"])


def flag_gnd_right(x, y):
    g = C["gnd"]
    out.append(f'<line x1="{x}" y1="{y}" x2="{x+22}" y2="{y}" stroke="{g}" stroke-width="2.5"/>')
    for i, half in enumerate((8, 5, 2)):
        xx = x + 22 + i * 4
        out.append(f'<line x1="{xx}" y1="{y-half}" x2="{xx}" y2="{y+half}" stroke="{g}" stroke-width="2"/>')
    text(x + 36, y + 4, "GND", 11, "start", "bold", g)


def render_wires():
    """Draw all wires; horizontal segments hop over other nets' vertical segments."""
    verticals = []
    for net, pts, _, _ in wires:
        for (x1, y1), (x2, y2) in zip(pts, pts[1:]):
            if x1 == x2:
                verticals.append((net, x1, min(y1, y2), max(y1, y2)))
    r = 6
    for net, pts, color, width in wires:
        d = f"M {pts[0][0]} {pts[0][1]}"
        for (x1, y1), (x2, y2) in zip(pts, pts[1:]):
            if y1 == y2 and x1 != x2:
                lo, hi = min(x1, x2), max(x1, x2)
                xs = sorted(
                    vx for vn, vx, vy1, vy2 in verticals
                    if vn != net and lo + r < vx < hi - r and vy1 < y1 < vy2
                )
                step = 1 if x2 > x1 else -1
                if step < 0:
                    xs.reverse()
                sweep = 1 if step > 0 else 0
                for xc in xs:
                    d += f" L {xc - r*step} {y1} A {r} {r} 0 0 {sweep} {xc + r*step} {y1}"
            d += f" L {x2} {y2}"
        out.append(
            f'<path d="{d}" fill="none" stroke="{color}" stroke-width="{width}" '
            f'stroke-linejoin="round" stroke-linecap="round"/>'
        )


# ---------------------------------------------------------------- header
text(30, 44, "FFB flight stick wiring", 22, weight="bold")

# ---------------------------------------------------------------- PC / USB
box(690, 96, 120, 36)
text(750, 119, "PC", 13, "middle", "bold")
out.append(f'<rect x="728" y="172" width="44" height="14" rx="4" fill="#ccc" stroke="{C["box"]}" stroke-width="1.5"/>')
wire("usb", [(750, 132), (750, 172)], C["usb"], 4)
text(758, 158, "USB", 10, color="#555")

# ---------------------------------------------------------------- Blackpill
BPx, BPy, BPw, BPh = 640, 186, 220, 574
box(BPx, BPy, BPw, BPh)
text(750, 210, "Blackpill", 14, "middle", "bold")
text(750, 226, "F411CEU6", 11, "middle", color="#555")

left_pins = [  # (y, label, color)
    (270, "PA8", C["pwm"]),
    (300, "PA9", C["pwm"]),
    (330, "PA10", C["pwm"]),
    (360, "PB12", C["en"]),
    (400, "GND", C["gnd"]),
    (580, "PB6", C["pwm"]),
    (610, "PB7", C["pwm"]),
    (640, "PB8", C["pwm"]),
    (670, "PB13", C["en"]),
    (705, "GND", C["gnd"]),
]
for y, label, col in left_pins:
    text(BPx + 10, y + 4, label, 11)

right_pins = [
    (250, "PA4", C["cs"]),
    (275, "PA5", C["sck"]),
    (300, "PA6", C["miso"]),
    (325, "PA7", C["mosi"]),
    (375, "PB10", C["cs"]),
    (425, "3V3", C["v33"]),
    (450, "GND", C["gnd"]),
    (700, "PB1", C["adc"]),
]
for y, label, col in right_pins:
    text(BPx + BPw - 10, y + 4, label, 11, "end")

# ---------------------------------------------------------------- shields
def shield(y0, n, motor, pads, sig_pins, ph_ys, pwr_edge):
    x0, w, h = 300, 220, 230
    box(x0, y0, w, h)
    ty = y0 + 22 if pwr_edge == "bottom" else y0 + 38
    text(x0 + w / 2, ty, f"SimpleFOC shield {n}", 13, "middle", "bold")
    for y, label, _ in sig_pins:
        text(x0 + w - 10, y + 4, label, 11, "end")
    text(x0 + 10, ph_ys[0] - 14, "motor", 10, color="#555")
    for y, ph in zip(ph_ys, "abc"):
        text(x0 + 10, y + 4, ph, 11)
    if pwr_edge == "bottom":
        text(350, y0 + h - 8, "+", 12, "middle", "bold", C["v12"])
        text(390, y0 + h - 8, "−", 12, "middle", "bold", C["gnd"])
        text(402, y0 + h - 9, "12V in", 10, "start", "normal", "#555")
    else:
        text(350, y0 + 18, "+", 12, "middle", "bold", C["v12"])
        text(390, y0 + 18, "−", 12, "middle", "bold", C["gnd"])
        text(402, y0 + 17, "12V in", 10, "start", "normal", "#555")


s1_pins = [(270, "D10 PWMA", C["pwm"]), (300, "D5 PWMB", C["pwm"]),
           (330, "D6 PWMC", C["pwm"]), (360, "D8 EN", C["en"]), (400, "GND", C["gnd"])]
s2_pins = [(580, "D9 PWMA", C["pwm"]), (610, "D3 PWMB", C["pwm"]),
           (640, "D11 PWMC", C["pwm"]), (670, "D7 EN", C["en"]), (705, "GND", C["gnd"])]
shield(200, 1, "M0", "A=D10 B=D5 C=D6 EN=D8", s1_pins, (280, 310, 340), "bottom")
shield(500, 2, "M1", "A=D9 B=D3 C=D11 EN=D7", s2_pins, (590, 620, 650), "top")
text(410, 750, "regulator pad open, don't connect VIN/5V to the blackpill", 10, "middle", color="#b00")

for pins, nets in ((s1_pins, "s1"), (s2_pins, "s2")):
    for i, (y, label, col) in enumerate(pins):
        wire(f"{nets}-{i}", [(520, y), (640, y)], col)

# ---------------------------------------------------------------- motors
def motor(cy, name, sensor, ph_ys):
    cx, r = 160, 55
    out.append(f'<circle cx="{cx}" cy="{cy}" r="{r}" fill="#ffffff" stroke="{C["box"]}" stroke-width="2"/>')
    out.append(f'<circle cx="{cx}" cy="{cy}" r="18" fill="none" stroke="#999" stroke-width="1.5" stroke-dasharray="3 3"/>')
    text(cx, cy - 22, name, 16, "middle", "bold")
    text(cx, cy + 40, "GM5208-24", 10, "middle", color="#555")
    for y, ph in zip(ph_ys, "ABC"):
        dy = y - cy
        x_edge = cx + math.sqrt(r * r - dy * dy)
        wire(f"{name}-{ph}", [(round(x_edge, 1), y), (300, y)], C["phase"], 4)
        text(250, y - 5, ph, 10, "middle", color="#555")


motor(310, "M0", "AS5047P #0", (280, 310, 340))
motor(620, "M1", "AS5047P #1", (590, 620, 650))

# ---------------------------------------------------------------- PSU + switch
box(30, 440, 180, 70)
text(120, 472, "12V 3A supply", 13, "middle", "bold")
text(200, 459, "+", 12, "end", "bold", C["v12"])
text(200, 499, "−", 12, "end", "bold", C["gnd"])
wire("v12", [(210, 455), (240, 455)], C["v12"], 3)
wire("v12b", [(276, 455), (350, 455)], C["v12"], 3)
wire("v12c", [(350, 430), (350, 500)], C["v12"], 3)
out.append(f'<circle cx="240" cy="455" r="3.5" fill="#fff" stroke="{C["v12"]}" stroke-width="2"/>')
out.append(f'<circle cx="276" cy="455" r="3.5" fill="#fff" stroke="{C["v12"]}" stroke-width="2"/>')
out.append(f'<line x1="242" y1="453" x2="272" y2="440" stroke="{C["v12"]}" stroke-width="3" stroke-linecap="round"/>')
text(258, 432, "switch", 10, "middle", "normal", C["v12"])
wire("gnd-psu", [(210, 495), (390, 495)], C["gnd"], 3)
wire("gnd-psu2", [(390, 430), (390, 500)], C["gnd"], 3)

# ---------------------------------------------------------------- AS5047P sensors
SX = 1060
def sensor(y0, h, n, motor, pin_rows, pwr_rows):
    box(SX, y0, 200, h)
    text(SX + 130, y0 + 26, f"AS5047P ({motor})", 13, "middle", "bold")
    for y, label in pin_rows:
        text(SX + 10, y + 4, label, 11)
    for y, label in pwr_rows:
        text(SX + 190, y + 4, label, 11, "end")


sensor(225, 165, 0, "M0", [(250, "CSn"), (275, "CLK"), (300, "MISO"), (325, "MOSI")], [(345, "VDD"), (370, "GND")])
sensor(455, 165, 1, "M1", [(480, "CLK"), (505, "MISO"), (530, "MOSI"), (555, "CSn")], [(575, "VDD"), (600, "GND")])

wire("cs0", [(860, 250), (SX, 250)], C["cs"])
wire("sck", [(860, 275), (SX, 275)], C["sck"])
wire("sck", [(1000, 275), (1000, 480), (SX, 480)], C["sck"])
wire("miso", [(860, 300), (SX, 300)], C["miso"])
wire("miso", [(980, 300), (980, 505), (SX, 505)], C["miso"])
wire("mosi", [(860, 325), (SX, 325)], C["mosi"])
wire("mosi", [(960, 325), (960, 530), (SX, 530)], C["mosi"])
wire("cs1", [(860, 375), (930, 375), (930, 555), (SX, 555)], C["cs"])

# ---------------------------------------------------------------- throttle pot
box(SX, 650, 200, 110)
text(SX + 125, 676, "Throttle pot", 13, "middle", "bold")
text(SX + 125, 694, "10k", 10, "middle", color="#555")
for y, label in ((670, "3"), (700, "2 wiper"), (730, "1")):
    text(SX + 10, y + 4, label, 11)
# resistor symbol with wiper arrow
zx, zy = SX + 125, 722
zz = " ".join(f"{zx - 40 + i * 8},{zy + (-6 if i % 2 else 6) if 0 < i < 10 else zy}" for i in range(11))
out.append(f'<polyline points="{zz}" fill="none" stroke="{C["box"]}" stroke-width="1.6"/>')
out.append(f'<line x1="{zx}" y1="{zy + 22}" x2="{zx}" y2="{zy + 9}" stroke="{C["adc"]}" stroke-width="1.6"/>')
out.append(f'<polygon points="{zx-4},{zy+12} {zx+4},{zy+12} {zx},{zy+6}" fill="{C["adc"]}"/>')
wire("adc", [(860, 700), (SX, 700)], C["adc"])

# ---------------------------------------------------------------- wires, then pins/junctions on top
render_wires()

for x, y in ((1000, 275), (980, 300), (960, 325), (350, 455), (390, 495)):
    color = {275: C["sck"], 300: C["miso"], 325: C["mosi"], 455: C["v12"], 495: C["gnd"]}[y]
    dot(x, y, color)
for y, _, col in left_pins:
    pin(BPx, y, col)
for y, _, col in right_pins:
    pin(BPx + BPw, y, col)
for pins in (s1_pins, s2_pins):
    for y, _, col in pins:
        pin(520, y, col)
for y in (280, 310, 340, 590, 620, 650):
    pin(300, y, C["phase"])
for x in (350, 390):
    pin(x, 430, C["v12"] if x == 350 else C["gnd"])
    pin(x, 500, C["v12"] if x == 350 else C["gnd"])
for y, col in ((250, C["cs"]), (275, C["sck"]), (300, C["miso"]), (325, C["mosi"]),
               (480, C["sck"]), (505, C["miso"]), (530, C["mosi"]), (555, C["cs"]), (700, C["adc"])):
    pin(SX, y, col)

flag_3v3_right(BPx + BPw, 425)
flag_gnd_right(BPx + BPw, 450)
for y in (345, 575):
    flag_3v3_right(SX + 200, y)
    pin(SX + 200, y, C["v33"])
for y in (370, 600):
    flag_gnd_right(SX + 200, y)
    pin(SX + 200, y, C["gnd"])
flag_3v3(SX, 670)
pin(SX, 670, C["v33"])
flag_gnd(SX, 730)
pin(SX, 730, C["gnd"])

svg = (
    f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}" '
    f'font-family="{FONT}">\n<rect width="{W}" height="{H}" fill="#ffffff"/>\n'
    + "\n".join(out)
    + "\n</svg>\n"
)
Path(__file__).with_name("wiring.svg").write_text(svg, encoding="utf-8")
print("wrote", Path(__file__).with_name("wiring.svg"))
