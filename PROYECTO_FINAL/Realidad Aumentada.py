import cv2
import numpy as np
import cv2.aruco as aruco
import requests

# credenciales blynk
token = "Q1-jgzuW2fAEJIo1WlKiG8NbUVJfIa60"
pinLuz = 'V6'
blynk_api_url1 = f'https://blynk.cloud/external/api/get?token={token}&{pinLuz}'

# Blynk: Obtener los valores de los sensores de luz y distancia
def obtener_valores_blynk():
    valorSensorLuz = requests.get(blynk_api_url1)
    valorLuz = valorSensorLuz.text
    return valorLuz

# Crear un objeto detector de marcadores
parametros = cv2.aruco.DetectorParameters()

# Crear un diccionario de marcadores ArUco
aruco_diccionario = aruco.getPredefinedDictionary(aruco.DICT_7X7_100)

# Inicializar la cámara (asegúrate de tener una cámara conectada)
captura = cv2.VideoCapture(0)

# Cargar imágenes diferentes para cada marcador
imagen_luz = cv2.imread("foco2.png")  # Imagen para el marcador de luz

# Asumimos que el marcador con id 0 es para la luz y el marcador con id 1 es para la distancia
valorLuz = ''

while True:
    # Capturar un fotograma o cuadro (frame) de la cámara
    lectura, frame = captura.read()
    
    # Convertir el fotograma a escala de grises
    cuadro_gris = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    # Crear un objeto de detector ArUco
    detector = aruco.ArucoDetector(aruco_diccionario, parametros)

    # Detectar marcadores ArUco
    esquinas, identificador, puntosRechazados = detector.detectMarkers(cuadro_gris)

    # Obtener los valores actuales de los sensores de Blynk
    valorLuz= obtener_valores_blynk()

    if identificador is not None:
        # Dibujar los marcadores detectados
        aruco.drawDetectedMarkers(frame, esquinas, identificador)
        
        for i in range(len(identificador)):
            # Obtener las esquinas del marcador actual
            marker_corners = esquinas[i][0]
                
            # Definir la posición y el tamaño de la superposición
            x, y, w, h = cv2.boundingRect(marker_corners)
            
            if identificador[i] == 0:
                # Para el marcador de luz, superponer la imagen de luz
                imagen_luz_resized = cv2.resize(imagen_luz, (w, h))
                frame[y:y+h, x:x+w] = imagen_luz_resized
                cv2.putText(frame, f"Luz: {valorLuz}", (x, y - 10), cv2.FONT_HERSHEY_COMPLEX_SMALL, 1, (0, 255, 0), 2)
    
    # Mostrar el resultado en una ventana
    cv2.imshow('Aruco', frame)
    
    # Salir con la tecla 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Liberar la cámara y cerrar la ventana
captura.release()
cv2.destroyAllWindows()