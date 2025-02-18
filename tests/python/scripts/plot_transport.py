import matplotlib.pyplot as plt
import pandas as pd
import argparse


from ODD_regions import plot_regions


def main(args):
    """
    @brief Plot the transport data for a single event in the z-r plane.
    """

    # Read data
    data = pd.read_json(args.input)

    # Create new figure
    fig, ax = plt.subplots()

    # Plot the ODD regions
    if args.region_composition:
        plot_regions(ax, args.region_composition)

    # Loop over the tracks
    for track_id in data.track_id.unique():
        track_data = data[data.track_id == track_id]
        # Plot the transport in the z-r plane
        ax.plot(track_data.z, track_data.r, "o--", ms=3, color="black", zorder=10)

    # Remember the track zoom region
    track_x_lim = ax.get_xlim()
    track_y_lim = ax.get_ylim()

    if args.track_zoom:
        # Legend not supported in this mode
        ax.set_xlim(track_x_lim)
        ax.set_ylim(track_y_lim)
    else:
        ax.set_ylim([0, 4])
        ax.set_xlim([-6.5, 6.5])

    ax.set_xlabel("Z [m]")
    ax.set_ylabel("R [m]")

    if args.label:
        ax.text(0.01, 0.95, args.label, transform=ax.transAxes)

    plt.savefig(args.output, dpi=300, bbox_inches="tight")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Plot particle transport data.")
    parser.add_argument("--input", type=str, required=True, help="Input file path")
    parser.add_argument("--output", type=str, required=True, help="Output file path")
    parser.add_argument(
        "--track_zoom", action="store_true", help="Zoom in to the track region"
    )
    parser.add_argument(
        "--region_composition",
        type=str,
        help="Plot the ODD regions by providing json file with region composition",
    )
    parser.add_argument("--label", type=str, required=False, help="Label for the plot")

    main(parser.parse_args())
