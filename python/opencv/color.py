# Created by Temkeng Thibaut at 27.07.2021
import cv2
import numpy as np


def on_change(val=None):
    """
    Anpassen die Farbewerte in Abhängigkeit der Schieberwerte.

    Parameters:
    val: Der Wert des Schiebers:
    """
    imageCopy = img.copy()
    rot = int(cv2.getTrackbarPos('Rot', windowName))
    blau = int(cv2.getTrackbarPos('Blau', windowName))
    gruen = int(cv2.getTrackbarPos('Gruen', windowName))
    imageCopy[..., 0] = blau
    imageCopy[..., 1] = gruen
    imageCopy[..., 2] = rot
    w, h, _ = imageCopy.shape
    spanne = h // 3
    args = {"fontFace": cv2.FONT_HERSHEY_SIMPLEX, "fontScale": 0.5, "thickness": 1}
    cv2.putText(imageCopy, "R:" + str(rot), (0, w - 10), color=(0, 0, 255), **args)
    cv2.putText(imageCopy, "B:" + str(blau), (100, w - 10), color=(255, 0, 0), **args)
    cv2.putText(imageCopy, "G:" + str(gruen), (50, w - 10), color=(0, 255, 0), **args)

    cv2.imshow("windowName", imageCopy)
    imageCopy[:] = 0
    imageCopy[:, 0:spanne, 2] = rot
    imageCopy[:, 2 * spanne:, 0] = blau
    imageCopy[:, spanne:2 * spanne, 1] = gruen
    cv2.imshow(windowName, imageCopy)


if __name__ == '__main__':
    img = np.zeros((500, 600, 3), dtype=np.uint8)
    windowName = 'RGB'
    # RGB-Farbe anzeigen
    cv2.imshow(windowName, img)
    # Farben-Schieber hinzufügen
    cv2.createTrackbar('Rot', windowName, 0, 255, on_change)
    cv2.createTrackbar('Gruen', windowName, 0, 255, on_change)
    cv2.createTrackbar('Blau', windowName, 0, 255, on_change)

    cv2.waitKey(0)
    cv2.destroyAllWindows()
