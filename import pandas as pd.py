import pandas as pd
import matplotlib.pyplot as plt

# CSV 파일 불러오기
df = pd.read_csv("Accelerometer.csv")

# x, y, z 축 데이터 추출
x = df["x"]
y = df["y"]
z = df["z"]

# norm² 계산
df["norm_squared"] = x**2 + y**2 + z**2

# 그래프 그리기
plt.figure(figsize=(12, 5))
plt.plot(df["seconds_elapsed"], df["norm_squared"])
plt.title("Norm² of Acceleration over Time")
plt.xlabel("Time (seconds)")
plt.ylabel("Norm² (x² + y² + z²)")
plt.grid(True)
plt.show()
