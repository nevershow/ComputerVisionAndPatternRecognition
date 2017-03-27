# -*- coding: utf-8 -*-
import os
from PIL import Image

if __name__ == '__main__':
  i = 0
  for file in os.listdir("."):
    if file.endswith(".jpg"):
      Image.open(file).save("%d.bmp" % i)
      i += 1
