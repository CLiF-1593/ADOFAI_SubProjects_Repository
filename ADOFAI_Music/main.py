import wave
import numpy as np
import matplotlib.pyplot as plt


def extract_frequency_data(wav_file):
    # WAV 파일 열기
    with wave.open(wav_file, 'r') as wav:
        # 파라미터 추출
        frames = wav.getnframes()
        rate = wav.getframerate()
        duration = frames / float(rate)

        # 오디오 프레임 읽기
        audio_data = wav.readframes(frames)

    # 오디오 데이터를 numpy 배열로 변환
    audio_data = np.frombuffer(audio_data, dtype=np.int16)

    # FFT (Fast Fourier Transform) 수행
    fft_result = np.fft.fft(audio_data)
    fft_freqs = np.fft.fftfreq(len(fft_result), 1.0 / rate)

    # 절반만 사용 (실제 주파수 데이터)
    half = len(fft_freqs) // 2
    fft_result = np.abs(fft_result[:half])
    fft_freqs = fft_freqs[:half]

    return fft_freqs, fft_result

def plot_frequency_data(wav_file):
    # 주파수 데이터 추출
    freqs, fft_result = extract_frequency_data(wav_file)

    # 그래프로 표현
    plt.figure(figsize=(12, 6))
    plt.plot(freqs, fft_result)
    plt.title(f"Frequency Spectrum of {wav_file}")
    plt.xlabel("Frequency (Hz)")
    plt.ylabel("Amplitude")
    plt.grid()
    plt.show()

# 이 함수는 WAV 파일의 경로를 입력으로 받아, 주파수 스펙트럼을 그래프로 표시합니다.
plot_frequency_data("R.wav")
