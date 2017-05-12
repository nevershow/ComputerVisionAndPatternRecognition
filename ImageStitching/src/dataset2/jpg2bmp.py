import os
from PIL import Image

i = 1
for img in os.listdir('.'):
  if img.endswith('bmp'):
    jpg = Image.open(img)
    jpg.save('%d.jpg' % i)
    i += 1
