"""Generate the figure dev-container-architecture.png.

Usage: python dev-container-architecture.py
"""
from pathlib import Path

import matplotlib.pyplot as plt
from matplotlib.patches import FancyArrowPatch, FancyBboxPatch

HOST_COLOR = "#dbe9f6"
CONTAINER_COLOR = "#dff0d8"
BOX_COLOR = "#ffffff"
EDGE_COLOR = "#333333"
ARROW_COLOR = "#c0392b"


def frame(ax, x, y, w, h, color, title):
    """Draw a large rounded frame with a title (host or container)."""
    ax.add_patch(FancyBboxPatch((x, y), w, h,
                                boxstyle="round,pad=0.02,rounding_size=0.3",
                                facecolor=color, edgecolor=EDGE_COLOR,
                                linewidth=1.5))
    ax.text(x + w / 2, y + h - 0.45, title, ha="center", va="center",
            fontsize=15, fontweight="bold")


def box(ax, x, y, w, h, title, subtitle=None):
    """Draw a component box and return its anchor points."""
    ax.add_patch(FancyBboxPatch((x, y), w, h,
                                boxstyle="round,pad=0.02,rounding_size=0.15",
                                facecolor=BOX_COLOR, edgecolor=EDGE_COLOR,
                                linewidth=1.2))
    if subtitle:
        ax.text(x + w / 2, y + h * 0.64, title, ha="center", va="center",
                fontsize=12, fontweight="bold")
        ax.text(x + w / 2, y + h * 0.30, subtitle, ha="center", va="center",
                fontsize=10, family="monospace")
    else:
        ax.text(x + w / 2, y + h / 2, title, ha="center", va="center",
                fontsize=12, fontweight="bold")
    return {"left": (x, y + h / 2), "right": (x + w, y + h / 2),
            "top": (x + w / 2, y + h), "bottom": (x + w / 2, y)}


def arrow(ax, start, end, label, both=False, label_offset=0.3):
    """Draw an arrow between two points with a label above it."""
    ax.add_patch(FancyArrowPatch(start, end,
                                 arrowstyle="<|-|>" if both else "-|>",
                                 mutation_scale=18, linewidth=2,
                                 color=ARROW_COLOR))
    ax.text((start[0] + end[0]) / 2, (start[1] + end[1]) / 2 + label_offset,
            label, ha="center", va="bottom", fontsize=10,
            color=ARROW_COLOR, style="italic")


def main():
    fig, ax = plt.subplots(figsize=(11, 5.2))
    ax.set_xlim(0, 22)
    ax.set_ylim(0, 10.4)
    ax.axis("off")

    # Host (left) and Docker container (right)
    frame(ax, 0.3, 0.3, 7.6, 9.6, HOST_COLOR, "Host")
    frame(ax, 12.0, 0.3, 9.7, 9.6, CONTAINER_COLOR, "Dev Container")

    ui = box(ax, 1.0, 5.8, 6.2, 2.0, "VS Code (UI)", "editor window")
    project = box(ax, 1.0, 1.4, 6.2, 2.0, "Project directory",
                  "source code")

    server = box(ax, 12.7, 6.6, 8.3, 1.9, "VS Code Server",
                 "+ extensions (C++, CMake)")
    tools = box(ax, 12.7, 4.0, 8.3, 1.9, "Tools",
                "g++, cmake, gdb, git")
    workspace = box(ax, 12.7, 1.4, 8.3, 2.0, "Mounted project",
                    "/workspaces/<project>")

    arrow(ax, ui["right"], (server["left"][0], ui["right"][1]),
          "remote connection", both=True)
    arrow(ax, project["right"], workspace["left"], "bind mount")
    ax.annotate("", xy=tools["top"], xytext=server["bottom"],
                arrowprops={"arrowstyle": "-|>", "color": EDGE_COLOR,
                            "linewidth": 1.2})
    ax.annotate("", xy=workspace["top"], xytext=tools["bottom"],
                arrowprops={"arrowstyle": "-|>", "color": EDGE_COLOR,
                            "linewidth": 1.2})

    out = Path(__file__).with_suffix(".png")
    fig.savefig(out, dpi=150, bbox_inches="tight", facecolor="white")
    print(f"Saved {out}")


if __name__ == "__main__":
    main()
