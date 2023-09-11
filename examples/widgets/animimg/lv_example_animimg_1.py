anim_images = [None]*3
# Create an image from the png file
try:
    with open('../../assets/animimage001.png','rb') as f:
        anim001_data = f.read()
except:
    print("Could not find animimage001.png")
    sys.exit()

anim_images[0] = lv.image_dsc_t({
  'data_size': len(anim001_data),
  'data': anim001_data
})

try:
    with open('../../assets/animimage002.png','rb') as f:
        anim002_data = f.read()
except:
    print("Could not find animimage002.png")
    sys.exit()

anim_images[1] = lv.image_dsc_t({
  'data_size': len(anim002_data),
  'data': anim002_data
})

try:
    with open('../../assets/animimage003.png','rb') as f:
        anim003_data = f.read()
except:
    print("Could not find animimage003.png")
    sys.exit()

anim_images[2] = lv.image_dsc_t({
  'data_size': len(anim003_data),
  'data': anim003_data
})

animimage0 = lv.animimage(lv.scr_act())
animimage0.center()
animimage0.set_src(anim_images, 3)
animimage0.set_duration(1000)
animimage0.set_repeat_count(lv.ANIM_REPEAT_INFINITE)
animimage0.start()



