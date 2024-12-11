import time
import numpy as np
from typing import List

def find_akf(signal: np.ndarray) -> np.ndarray:
    reversed_signal = signal[::-1]
    akf = np.correlate(signal, signal, mode='full')
    return akf[len(signal)-1:]

def generate_signals(N: int) -> np.ndarray:
    vars = int(2 ** N) - 1
    signals = np.full((vars, N), -1, dtype=int)
    for i in range(1, vars + 1):
        signals[i - 1] = add_one(signals[i - 2].copy(), N)
    return signals

def add_one(signal: np.ndarray, N: int) -> np.ndarray:
    i = N - 1
    while i >= 0 and signal[i] == 1:
        signal[i] = -1
        i -= 1
    if i >= 0:
        signal[i] = 1
    return signal

def max_sidelobe(akf: np.ndarray) -> int:
    sidelobes = np.abs(akf[1:])
    return np.max(sidelobes, initial=0)

def main():
    with open("exec_time_libs_py.txt", "w") as outfile:
        for signal_length in range(3, 21):

            vars = int(2 ** signal_length) - 1
            best_sidelobe = float('inf')
            optimal_signal = None
            optimal_akf = None

            signals = generate_signals(signal_length)
            start_time = time.time()

            for signal in signals:
                akf = find_akf(signal)
                temp_sidelobe = max_sidelobe(akf)
                if temp_sidelobe < best_sidelobe:
                    optimal_akf = akf
                    optimal_signal = signal
                    best_sidelobe = temp_sidelobe

            elapsed_time = time.time() - start_time
            outfile.write(f"{signal_length} {elapsed_time}\n")
            print(f"{signal_length}, Time: {elapsed_time:.2f} seconds")

if __name__ == "__main__":
    main()
