import numpy as np
import scipy.fftpack

# Number of points in the signal
N = 1024

# Generate white noise
white_noise = np.random.randn(N)

# Create a Hanning window
hanning_window = np.hanning(N)

# Apply the Hanning window to the white noise
windowed_white_noise = white_noise * hanning_window

# Compute the FFT of the windowed white noise
spectrum = np.abs(scipy.fftpack.fft(windowed_white_noise))

# Compute the spectral flatness
geometric_mean = np.exp(np.mean(np.log(spectrum + 1e-10)))
arithmetic_mean = np.mean(spectrum)
spectral_flatness = geometric_mean / arithmetic_mean

print(spectral_flatness)
