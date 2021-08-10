# Created by Temkeng Thibaut at 28.07.2021
import cv2
import cv2 as cv
import numpy as np
from matplotlib import pyplot as plt


def imshow(img, title=""):
    plt.figure()
    plt.axis("OFF")
    plt.title(title)
    plt.imshow(img, "gray")


def norm(img, new_min=0, new_max=255):
    img_min, img_max = img.min(), img.max()
    distance = max(img_max - img_min, 1)
    return (new_min + ((img - img_min) / distance) * (new_max - new_min)).astype(type(img_max))


def histogram(img, title=""):
    plt.figure()
    plt.title(title)
    hist = cv.calcHist([img], [0], None, [256], [0, 256])
    plt.plot(hist)


def plot_y(*datas, title=""):
    plt.figure()
    plt.title(title)
    for data in datas:
        plt.plot(list(reversed(data)), range(len(data)))
def plot(*datas, title=""):
    plt.figure()
    plt.title(title)
    for data in datas:
        plt.plot(data)