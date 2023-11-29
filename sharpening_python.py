import cv2
import numpy as np

def robert_filter(image):
    h = image.shape[0]
    w = image.shape[1]
    image_new = np.zeros(image.shape, np.uint8)
    for i in range(1, h - 1):
        for j in range(1, w - 1):
            image_new[i][j] = np.abs(image[i][j].astype(int) - image[i + 1][j + 1].astype(int)) + np.abs(image[i + 1][j].astype(int) - image[i][j + 1].astype(int))
            image[i][j]-=image_new[i][j]*4
    return image
src=cv2.imread("C:/opencv/pictures/6.bmp")
cv2.imshow("src",src)
dst=robert_filter(src)
cv2.imshow("dst",dst)
cv2.waitKey()
cv2.destroyAllWindows()