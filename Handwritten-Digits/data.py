import tensorflow as tf
import numpy as np

mnist = tf.keras.datasets.mnist
(train_images, train_labels), (test_images, test_labels) = mnist.load_data()


train_images = train_images / 255.0
test_images = test_images / 255.0


def image_to_ascii(image, scale=1):
    height, width = image.shape
    scaled_height = height // scale
    scaled_width = width // scale

    ascii_image = [[" " for _ in range(scaled_width)] for _ in range(scaled_height)]

    for i in range(scaled_height):
        for j in range(scaled_width):
            start_i = i * scale
            end_i = (i + 1) * scale
            start_j = j * scale
            end_j = (j + 1) * scale

            avg = np.mean(image[start_i:end_i, start_j:end_j])

            ascii_image[i][j] = "*" if avg > 0.5 else "."

    return "\n".join("".join(row) for row in ascii_image)


for count in range(60000):
    ascii_art = image_to_ascii(train_images[count], scale=1)
    filename = "./datatmp/data" + str(count + 1) + ".txt"
    with open(filename, "w") as f:
        f.write(str(train_labels[count]) + "\n")
        f.write(ascii_art + "\n")
        f.close()

for count in range(10000):
    ascii_art = image_to_ascii(test_images[count], scale=1)
    filename = "./datatmp/data" + str(count + 1) + "_test.txt"
    with open(filename, "w") as f:
        f.write(str(test_labels[count]) + "\n")
        f.write(ascii_art + "\n")
        f.close()
