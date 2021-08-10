# Created by Temkeng Thibaut at 27.07.2021
import time

import numpy as np

from utils import *
from icecream import ic
from collections import defaultdict

ic.disable()
def get_energy(img, ksize=3, scale=1, delta=0, ddepth=cv.CV_16S):
    args = {"scale": scale, "delta": delta, "borderType": cv.BORDER_DEFAULT}
    # Smooth image to reduce noise or high frequency
    img = cv.GaussianBlur(img, (3, 3), 0)
    # Gradient X
    grad_x = cv.Sobel(img, ddepth, 1, 0, ksize=ksize, **args)
    # Gradient-Y
    grad_y = cv.Sobel(img, ddepth, 0, 1, ksize=ksize, **args)

    abs_grad_x = cv.convertScaleAbs(grad_x)
    abs_grad_y = cv.convertScaleAbs(grad_y)

    l2_energy = (grad_x ** 2 + grad_y ** 2) ** .5
    avg_energy = cv.addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0)  # (abs_grad_x + abs_grad_y)/2
    energy_sum = abs_grad_x + abs_grad_y
    return avg_energy, cv.convertScaleAbs(l2_energy), energy_sum


def _seam(img, energy=None):
    direction = [(255, 0, 0), (0, 255, 0), (0, 0, 255)]
    r, c = img.shape[:2]
    if energy is None:
        _, energy, _ = get_energy(img)
    # Brightness
    if len(energy.shape) == 3:
        energy = np.sum(energy, -1)
    energy_map = energy.copy()
    backtrack = np.zeros_like(energy, dtype=np.int)
    directions = np.zeros(img.shape, dtype=np.uint8)
    for i in range(r - 2, -1, -1):
        for j in range(0, c):
            left, right = max(0, j - 1), min(j + 2, c)
            local_energy = energy_map[i + 1, left:right]
            index = np.argmin(local_energy)
            backtrack[i, j] = index + left
            energy_map[i, j] += local_energy[index]
            directions[i, j] = direction[index + (len(local_energy) == 2)]
    return norm(energy_map).astype(np.uint8), backtrack, energy


def draw_seam_at(img, next_elements, index, energy_map=None, window_name="Energy Map"):
    next_element = 0
    result = img.copy()
    mask = np.zeros_like(next_elements, dtype=np.bool)
    for row in range(len(img) - 1):
        if row == 0:
            result[0, index] = (255, 0, 0)
            mask[row, index] = False
            next_element = next_elements[row, index]

        result[row + 1, next_element] = (255, 0, 0)
        mask[row + 1, next_element] = False
        next_element = next_elements[row, next_element]

    cv.imshow(windowName, cv.cvtColor(result, cv.COLOR_RGB2BGR))
    return result


def seam_carve_color(img, numberOfColumns=100, random=False):
    img_copy = img.copy()
    r, c = img.shape[:2]
    energy_map, backtrack, _ = _seam(img)
    if not random:
        min_indexes = np.argsort(energy_map[0])[:numberOfColumns]
    else:
        min_indexes = np.random.choice(c, numberOfColumns, replace=False)
    for min_index in np.append(min_indexes, 0):
        for i in range(r):
            img_copy[i, min_index] = (255, 0, 0)
            min_index = backtrack[i, min_index]
    return img_copy


def drop_columns(img, numberOfColumns=100):
    img_copy = img.copy()
    energy = None
    for _ in range(numberOfColumns):
        r, c = img_copy.shape[:2]
        energy_map, backtrack, energy = _seam(img_copy, energy)
        mask = np.ones((r, c), dtype=np.bool)
        # Find path with minimal energy
        min_index = np.argmin(energy_map[0])
        for i in range(r):
            mask[i, min_index] = False  # Mark the pixels for deletion
            img_copy[i, min_index] = (0, 0, 255)
            min_index = backtrack[i, min_index]
        # Remove the path with the minimal energy
        energy = energy[mask].reshape(r, -1)
        mask = np.stack([mask] * 3, axis=2)
        img_copy = img_copy[mask].reshape((r, c - 1, 3))
    return img_copy


def show_seam(img, c=500):
    energy_map, backtrack, energy = _seam(img)
    cv.imshow(windowName, cv.cvtColor(img, cv.COLOR_RGB2BGR))
    cv.imshow("Energy Map", cv.convertScaleAbs(norm(energy_map).astype(np.uint8)))
    f = lambda column: draw_seam_at(img, backtrack, max(column - 1, 0), energy_map)
    min_index = np.argmin(energy_map[0])
    f(min_index)
    cv2.createTrackbar('Column', windowName, min_index + 1, c, lambda column: f(column))


def to_delete(img):
    delete = defaultdict(list)
    img_copy = img.copy()
    r, c = img.shape[:2]
    seam_ = [0 for _ in range(r)]
    energy = None
    for col in range(img.shape[1]):
        r, c = img_copy.shape[:2]
        energy_map, backtrack, energy = _seam(img_copy, energy)
        mask = np.ones((r, c), dtype=np.bool)
        # Find path with minimal energy
        min_index = np.argmin(energy_map[0])
        print(col, min_index)
        for i in range(r):
            seam_[i] = min_index
            mask[i, min_index] = False  # Mark the pixels for deletion
            img_copy[i, min_index] = (0, 0, 255)
            min_index = backtrack[i, min_index]
        plot(seam_)
        plt.show()
        return
        delete[col] = seam_
        # Remove the path with the minimal energy
        energy = energy[mask].reshape(r, -1)
        mask = np.stack([mask] * 3, axis=2)
        img_copy = img_copy[mask].reshape((r, c - 1, 3))
    return delete


def shrink_n(img, c=500):
    '''' Remove columns'''
    def shrink(numberOfColumns):
        global energy_first, energy
        img_copy = img.copy()
        ic(numberOfColumns)
        for n in range(numberOfColumns):
            elapse_time = time.time()
            r, c = img_copy.shape[:2]
            if delete.get(n) is not None:
                if n == 0:
                    energy = energy_first.copy()
                seam = delete[n]
                mask = np.eye(r, c, dtype=np.bool)[seam]
                energy = energy[mask == False].reshape(r, c - 1)
                img_copy = img_copy[mask == False].reshape(r, c - 1, 3)
            else:
                energy_map, backtrack, energy = _seam(img_copy, energy)
                if n == 0:
                    energy_first = energy.copy()
                mask = np.ones((r, c), dtype=np.bool)
                mask[:] = True
                # Find path with minimal energy
                min_index = np.argmin(energy_map[0])
                seam = np.array([], dtype=np.int)
                for i in range(r):
                    seam = np.append(seam, min_index)
                    mask[i, min_index] = False  # Mark the pixels for deletion
                    img_copy[i, min_index] = (0, 0, 255)
                    min_index = backtrack[i, min_index]
                # Remove the path with the minimal energy
                draw_seam_at(img_copy, backtrack, np.argmin(energy_map[0]), energy_map, windowName)
                energy = energy[mask].reshape(r, -1)
                img_copy = img_copy[mask].reshape((r, c - 1, 3))
                delete[n] = seam
            elapse_time = time.time() - elapse_time

            ic(elapse_time)

            # cv.imshow("Shrink n", norm(energy_map).astype(np.uint8))
            # cv.imshow(windowName, cv.cvtColor(img_copy, cv.COLOR_RGB2BGR))

    img_copy = img.copy()
    cv.imshow(windowName, cv.cvtColor(img, cv.COLOR_RGB2BGR))
    delete = defaultdict(type(img))
    r, c = img_copy.shape[:2]
    # energy_first = np.zeros((r, c), dtype=np.int)


    cv.createTrackbar('Columns', windowName, 1, c - 1, lambda numberOfColumns: shrink(numberOfColumns))

    return img_copy


if __name__ == '__main__':
    energy = None
    energy_first = None
    windowName = "Seam Carving"
    filename = "data/g-star-shoe.jpg"
    # filename = "data/image.jpg"
    ic(filename)
    img = cv.imread(filename, cv.IMREAD_COLOR)
    img = cv.cvtColor(img, cv.COLOR_BGR2RGB)
    r, c, _ = img.shape
    img = cv.resize(img, (min(r, 500), min(c, 500)))  # Reduce the size to accelerate the computation
    r, c, _ = img.shape
    # imshow(img, "Original Image Shape=" + str(img.shape))
    # elapse_time = time.time()
    # drop_c = drop_columns(img, 100)
    # elapse_time = time.time() - elapse_time
    # imshow(drop_c, "Seam Drop Columns Shape=" + str(drop_c.shape))
    # show_seam(img, c)
    shrink_n(img)
    # delete = to_delete(img)
    plt.show()
    cv.waitKey(0)
    cv.destroyAllWindows()
