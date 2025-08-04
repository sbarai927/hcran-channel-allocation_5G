# plot_metrics.py (excerpt)
import matplotlib.pyplot as plt

# Sample data from the paper:contentReference[oaicite:34]{index=34}:
frequencies = ["24 GHz", "60 GHz"]
ap_power_static = [380, 480]    # in mW (Existing method)
ap_power_dynamic = [480, 600]   # in mW (Proposed method)
diversity_static = [8, 14]      # unitless diversity gain
diversity_dynamic = [12, 18]
delay_static = [600, 1000]      # in ms (End-to-End delay)
delay_dynamic = [300, 800]

# Plot AP Power comparison
plt.figure(figsize=(4,3))
indices = range(len(frequencies))
plt.bar([i-0.2 for i in indices], ap_power_static, width=0.4, label='Static')
plt.bar([i+0.2 for i in indices], ap_power_dynamic, width=0.4, label='Dynamic')
plt.xticks(indices, frequencies); plt.ylabel("AP Power (mW)"); plt.title("AP Power Comparison")
plt.legend(); plt.tight_layout(); plt.savefig("images/AP_power_comparison.png")

# Plot Diversity Gain comparison
plt.figure(figsize=(4,3))
plt.bar([i-0.2 for i in indices], diversity_static, width=0.4, label='Static')
plt.bar([i+0.2 for i in indices], diversity_dynamic, width=0.4, label='Dynamic')
plt.xticks(indices, frequencies); plt.ylabel("Diversity Gain"); plt.title("Diversity Gain Comparison")
plt.legend(); plt.tight_layout(); plt.savefig("images/diversity_gain_comparison.png")

# Plot E2E Delay comparison
plt.figure(figsize=(4,3))
plt.bar([i-0.2 for i in indices], delay_static, width=0.4, label='Static')
plt.bar([i+0.2 for i in indices], delay_dynamic, width=0.4, label='Dynamic')
plt.xticks(indices, frequencies); plt.ylabel("End-to-End Delay (ms)"); plt.title("E2E Delay Comparison")
plt.legend(); plt.tight_layout(); plt.savefig("images/e2e_delay_comparison.png")
