import textwrap

sweep_delay_py = textwrap.dedent("""
    #!/usr/bin/env python3
    \"""
    sweep_delay.py

    Generate a line chart of mean end-to-end delay vs. sensing period
    Usage:
        python3 sweep_delay.py results/2s/ results/1s/ results/0.5s/ results/0.25s/
    Assumes each result folder contains a 'delay_summary.csv' with a column 'delay_ms'.
    \"""

    import sys
    import os
    import pandas as pd
    import matplotlib.pyplot as plt

    def load_mean_delay(path):
        csv = os.path.join(path, 'delay_summary.csv')
        if not os.path.isfile(csv):
            raise FileNotFoundError(f"Expected {{csv}} in {{path}}")
        df = pd.read_csv(csv)
        if 'delay_ms' not in df.columns:
            raise KeyError(f"'delay_ms' column missing in {{csv}}")
        return df['delay_ms'].mean()

    def main(paths):
        labels, means = [], []
        for p in paths:
            label = os.path.basename(os.path.normpath(p))
            mean_delay = load_mean_delay(p)
            labels.append(label)
            means.append(mean_delay)

        # Sort by numeric value if labels are numeric-like
        try:
            labels, means = zip(*sorted(zip(labels, means), key=lambda x: float(x[0].rstrip('s'))))
        except:
            pass

        plt.figure(figsize=(6,4))
        plt.plot(labels, means, marker='o')
        plt.xlabel('Sensing period')
        plt.ylabel('Mean E2E delay (ms)')
        plt.title('Delay vs. RRH Sensing Interval')
        plt.grid(True)
        plt.tight_layout()
        out = 'images/sweep_delay.png'
        plt.savefig(out, dpi=300)
        print(f"Saved plot to {{out}}")

    if __name__ == '__main__':
        if len(sys.argv) < 2:
            print("Usage: python3 sweep_delay.py <result_dir1> <result_dir2> ...")
            sys.exit(1)
        main(sys.argv[1:])
""")

with open('scripts/sweep_delay.py', 'w') as f:
    f.write(sweep_delay_py)

sweep_delay_py
