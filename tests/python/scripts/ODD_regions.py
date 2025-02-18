import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
import json
from pathlib import Path

plt.rcParams["ytick.right"] = plt.rcParams["xtick.top"] = True
plt.rcParams["xtick.direction"] = "in"
plt.rcParams["ytick.direction"] = "in"
plt.rcParams["font.size"] = 12.0
plt.rcParams["font.family"] = "sans-serif"
plt.rcParams["legend.frameon"] = False
plt.rcParams["legend.columnspacing"] = 0.2
plt.rcParams["legend.handletextpad"] = 0.2
plt.rcParams["legend.labelspacing"] = 0.2
plt.rcParams["legend.borderpad"] = 0
plt.rcParams["legend.handlelength"] = 1.0


def plot_regions(ax, config_path):
    """
    Plots detector regions from a given JSON configuration file onto a provided axis.

    Parameters:
        ax (matplotlib.axes.Axes): The axis on which to plot.
        config_path (str or Path): Path to the JSON configuration file.
    """

    names = {
        "beampipe": "Beam pipe",
        "sstrips": "Short Strips",
        "lstrips": "Long Strips",
        "pixel": "Pixel",
        "solenoid": "Solenoid",
        "ecalbarrel": "ECal barrel",
        "ecalendcap": "ECal endcap",
        "hcalbarrel": "HCal barrel",
        "hcalendcap": "HCal endcap",
    }

    with open(config_path, "r") as f:
        config = json.load(f)

    zmax = float("-inf")
    zmin = float("inf")
    rmax = 0

    u_scale = 1 / 1000  # m
    ax.set_xlabel("z [m]")
    ax.set_ylabel("r [m]")

    labels = set()

    for key, boxes in config.items():
        for box in boxes:
            label = names[key] if key not in labels else None
            labels.add(key)
            ax.add_patch(
                Rectangle(
                    (box["zmin"] * u_scale, box["rmin"] * u_scale),
                    box["zmax"] * u_scale - box["zmin"] * u_scale,
                    box["rmax"] * u_scale - box["rmin"] * u_scale,
                    fc=box["color"],
                    label=label,
                )
            )

            zmax = max(zmax, box["zmax"] * u_scale)
            zmin = min(zmin, box["zmin"] * u_scale)
            rmax = max(rmax, box["rmax"] * u_scale)

    ax.set_xlim(zmin, zmax)
    ax.set_ylim(0, 1.25 * rmax)
    ax.legend(ncol=3)


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description="Make material composition plots")
    parser.add_argument("config", type=Path, help="Input config file with boxes")
    args = parser.parse_args()

    fig, ax = plt.subplots()
    plot_regions(ax, args.config)
    plt.show()
