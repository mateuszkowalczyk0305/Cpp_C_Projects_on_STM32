import hid
import tkinter as tk
from tkinter import ttk
import pygame

# Znajdujemy urządzenie HID (należy dostosować vendor_id i product_id zgodnie z ustawieniami joysticka)
VENDOR_ID = 1155  # Wprowadź swoje vendor_id
PRODUCT_ID = 22352  # Wprowadź swoje product_id

# Inicjalizacja pygame dla obsługi joysticka
pygame.init()
pygame.joystick.init()

# Zmienna do przechowywania joysticka
joystick = None


# Funkcja do aktualizacji informacji o podłączonym urządzeniu
def update_device_info():
    global joystick, joystick_name, joystick_type

    # Sprawdzamy, czy jest podłączony joystick
    if pygame.joystick.get_count() > 0:
        if not joystick:  # Jeśli joystick jeszcze nie został zainicjalizowany
            joystick = pygame.joystick.Joystick(0)
            joystick.init()
            joystick_name = joystick.get_name()
            joystick_type = "Joystick"
            device_info_label.config(text=f"Podłączone urządzenie: {joystick_name} ({joystick_type})")
    else:
        if joystick:  # Jeżeli joystick był wcześniej podłączony
            joystick.quit()  # Wyłącz joystick
            joystick = None
            device_info_label.config(text="Brak podłączonego joysticka")

    root.after(1000, update_device_info)  # Powtarzaj co 1 sekundę


# Funkcja wysyłania danych do joysticka HID
def send_data_to_joystick(data):
    try:
        device = hid.device()
        device.open(VENDOR_ID, PRODUCT_ID)  # Otwórz urządzenie
        report = data
        device.write(report)  # Wyślij dane
        device.close()  # Zamknij po wysłaniu
    except Exception as e:
        print(f"Nie udało się wysłać danych: {e}")


# Funkcja dla przycisku Green LED
def green_led():
    send_data_to_joystick([0, 1])  # Wysyłamy dane włączające zieloną diodę


# Funkcja dla przycisku Red LED
def red_led():
    send_data_to_joystick([0, 2])  # Wysyłamy dane włączające czerwoną diodę


# Funkcja do wyłączania diod
def turn_off_leds():
    send_data_to_joystick([0, 0])  # Wysyłamy dane wyłączające obie diody


# Funkcja do obsługi joysticka przez pygame
def handle_joystick_events():
    pygame.event.pump()  # Odświeżanie zdarzeń pygame
    if joystick:
        # Oś X
        x_axis = joystick.get_axis(0) * 100
        # Oś Y
        y_axis = joystick.get_axis(1) * 100
        # Przyciski joysticka
        button_pressed = joystick.get_button(0)

        # Aktualizujemy wartości sliderów
        x_slider.set(int(x_axis))
        y_slider.set(int(y_axis))

        # Zmieniamy kolor przycisku, gdy joystickowy przycisk jest naciśnięty
        if button_pressed:
            button_status.config(bg="green")
            turn_off_leds()  # Wyłączamy diody, gdy przycisk joysticka jest naciśnięty
        else:
            button_status.config(bg="red")


# Tworzymy okno aplikacji przy użyciu tkinter
root = tk.Tk()
root.title("Joystick LED Controller")

# Dodajemy informację o nazwie joysticka i jego typie na górze okna
device_info_label = tk.Label(root, text="Brak podłączonego joysticka", font=("Comic Sans MS", 14))
device_info_label.pack(pady=10)

# Dodajemy przyciski do sterowania diodami
green_button = tk.Button(root, text="Green LED", command=green_led)
green_button.pack(pady=10)

red_button = tk.Button(root, text="Red LED", command=red_led)
red_button.pack(pady=10)

# Dodajemy slidery do obsługi osi X i Y
x_slider_label = tk.Label(root, text="Oś X")
x_slider_label.pack()

x_slider = ttk.Scale(root, from_=-100, to=100, orient='horizontal', length=300)
x_slider.pack(pady=10)

y_slider_label = tk.Label(root, text="Oś Y")
y_slider_label.pack()

y_slider = ttk.Scale(root, from_=-100, to=100, orient='horizontal', length=300)
y_slider.pack(pady=10)

# Dodajemy przycisk do wskaźnika przycisku joysticka
button_status = tk.Label(root, text="Joystick Button", bg="red", width=20, height=2)
button_status.pack(pady=10)


# Dodajemy pętlę do obsługi zdarzeń pygame (ruchy joysticka)
def update_joystick():
    handle_joystick_events()  # Wywołaj funkcję do obsługi joysticka
    root.after(50, update_joystick)  # Powtarzaj co 50ms


# Uruchamiamy obsługę joysticka
update_joystick()

# Uruchamiamy funkcję sprawdzającą podłączenie joysticka
update_device_info()

# Uruchamiamy aplikację tkinter
root.mainloop()

# Zakończenie pygame
pygame.quit()
