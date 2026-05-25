#!/usr/bin/env python3
"""
plot_filter.py — visualize EMA filter output.

Reads a CSV produced by filter_tool (columns: t, raw, filtered) and
plots the raw signal against the filtered one so the smoothing
behaviour can be inspected visually.

Also accepts a 2-column CSV (t, value) — handy for previewing the
raw input on its own before filtering.

Usage:
    python plot_filter.py <csv_file>                  # interactive window
    python plot_filter.py <csv_file> --save out.png   # write PNG instead
    python plot_filter.py <csv_file> --title "alpha=0.12"
"""

import argparse
import csv
import sys
from pathlib import Path

import matplotlib.pyplot as plt


def read_csv(path: Path):
    """Return (t, raw, filtered_or_None) from a 2- or 3-column CSV."""
    with path.open(newline="") as f:
        reader = csv.reader(f)
        header = next(reader)
        ncols = len(header)
        if ncols not in (2, 3):
            raise ValueError(
                f"{path}: expected 2 or 3 columns, got {ncols}: {header!r}"
            )

        t, raw, filtered = [], [], []
        for row in reader:
            t.append(float(row[0]))
            raw.append(float(row[1]))
            if ncols == 3:
                filtered.append(float(row[2]))

    return t, raw, (filtered if filtered else None)


def plot(t, raw, filtered, title: str, save: Path | None) -> None:
    fig, ax = plt.subplots(figsize=(12, 5))

    ax.plot(t, raw, label="raw",
            color="lightgray", linewidth=0.9, alpha=0.9)

    if filtered is not None:
        ax.plot(t, filtered, label="filtered",
                color="#1f77b4", linewidth=1.8)

    ax.set_xlabel("t (s)")
    ax.set_ylabel("value")
    ax.set_title(title)
    ax.legend(loc="best")
    ax.grid(True, alpha=0.3)
    fig.tight_layout()

    if save is not None:
        fig.savefig(save, dpi=120)
        print(f"Saved: {save}")
    else:
        plt.show()


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Plot raw vs filtered ADC signal from filter_tool output."
    )
    parser.add_argument("csv_file", type=Path,
                        help="CSV file (t,raw,filtered) or (t,value)")
    parser.add_argument("--save", type=Path, default=None,
                        help="Write PNG to this path instead of showing window")
    parser.add_argument("--title", type=str, default=None,
                        help="Plot title (defaults to file name)")
    args = parser.parse_args()

    if not args.csv_file.is_file():
        print(f"Error: {args.csv_file} not found", file=sys.stderr)
        return 1

    t, raw, filtered = read_csv(args.csv_file)
    plot(t, raw, filtered,
         title=args.title or args.csv_file.name,
         save=args.save)
    return 0


if __name__ == "__main__":
    sys.exit(main())
