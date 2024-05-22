#Executar Script (>python thermal_camera.py)

import serial
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from scipy.ndimage import zoom, gaussian_filter

# Configure a porta serial (substitua 'COM3' pela porta correta)
try:
    ser = serial.Serial('COM3', 9600)  # Substitua 'COM3' pela porta serial correta
except serial.SerialException as e:
    print(f"Não foi possível abrir a porta serial: {e}")
    exit()

def read_data():
    try:
        # Ler uma linha de dados da porta serial
        line = ser.readline().decode('utf-8').strip()
        # Converter a linha de string para uma lista de floats
        data = [float(val) for val in line.split(',')]
        # Transformar a lista em uma matriz 8x8
        return np.array(data).reshape((8, 8))
    except ValueError as e:
        print(f"Erro ao converter dados: {e}")
        return np.zeros((8, 8))  # Retorna uma matriz vazia em caso de erro

def normalize_data(data):
    min_val = np.min(data)
    max_val = np.max(data)
    normalized = (data - min_val) / (max_val - min_val)
    return normalized

def improve_image(data, scale=10):
    # Normalizar os dados
    normalized_data = normalize_data(data)
    # Interpolar a imagem para aumentar a resolução
    high_res = zoom(normalized_data, scale, order=3)  # order=3 usa interpolação bicúbica
    # Aplicar filtro gaussiano para suavizar a imagem
    smoothed = gaussian_filter(high_res, sigma=1)
    return smoothed


# Configurar a visualização
fig, ax = plt.subplots()
initial_data = read_data()
print("Initial Data:\n", initial_data)  # Adicionado para depuração
improved_data = improve_image(initial_data)
print("Improved Data:\n", improved_data)  # Adicionado para depuração
therm_img = ax.imshow(improved_data, cmap='inferno', interpolation='nearest')

def update(frame):
    # Ler novos dados e melhorar a imagem
    data = read_data()
    print("Read Data:\n", data)  # Adicionado para depuração
    improved_data = improve_image(data)
    print("Improved Data:\n", improved_data)  # Adicionado para depuração
    therm_img.set_data(improved_data)
    return [therm_img]

# Criar a animação
ani = FuncAnimation(fig, update, interval=100)

# Mostrar a imagem
plt.show()
