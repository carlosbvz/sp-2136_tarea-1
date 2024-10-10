import numpy as np
import matplotlib.pyplot as plt

serial_results = np.loadtxt("log_serial.out")
omp_results = np.loadtxt("log_CILK.out")
omp_results = omp_results.reshape(-1,4)
print(omp_results)

#scaling
plt.plot(omp_results[:, 0], omp_results[:,1:].mean(axis=1), marker=".")
plt.xlabel("Number of threads")
plt.ylabel("Mean execution Time [s]")
plt.title("CPU: Intel Xeon Processor (Skylake)")
plt.grid(alpha=0.5)
plt.savefig("cilk_scaling.png")
plt.close()

#speedup
plt.plot(np.arange(1,5), np.arange(1,5), label="Ideal scaling")
plt.plot(omp_results[:, 0], serial_results.mean()/omp_results[:,1:].mean(axis=1), marker=".", label="CILK Tasks")
plt.xlabel("Number of threads")
plt.title("CPU: Intel Xeon Processor (Skylake)")
plt.ylabel("Speed Up")
plt.legend()
plt.grid(alpha=0.5)
plt.savefig("cilk_speedup.png")
plt.close()