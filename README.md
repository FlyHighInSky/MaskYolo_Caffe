# YOLO Caffe version with MaskRCNN

### Caffe-MaskYolo

#### What I add in this version of caffe?
- [x] Demos for object detection, mask segmentation and keypoints recognition
- [x] YOLO v2 (RegionLossLayer) and v3 (YoloLossLayer) are supported
- [x] Instance Mask segmentation with Yolo
- [x] Keypoints Recognition with yolo
- [x] training data preparation and training

#### preparation
```
# clone
git clone https://github.com/leon-liangwu/MaskYolo_Caffe.git --recursive

# install requirements
cd ROOT_MaskYolo
pip install -r requirements.txt

# compile box_utils
cd lib/box_utils
python setup.py build_ext --inplace

# compile caffe
cd caffe-maskyolo
cp Makefile.config.example Makefile.config
make -j
make pycaffe
```

#### download pretrained models
Click [here](https://www.dropbox.com/s/z1w2z8ya28v3lah/models.tgz?dl=0 "pretrained models") to download pretrained models
```
cd ROOT_MaskYolo
tar zxvf /your/downlaod/model/path/models.tgz ./
```


### Object Detection with YOLO
support to use yolo v2 or v3 to detect objects in images

#### objection demo
```
cd tools
python yolo_inference.py [--img_path=xxx.jpg] [--model=xxx.prototxt] [--weights=xxx.caffemodel]
# Net forward time consumed: 3.96ms
```
The demo result is shown below.

![](assets/detection1.png)

#### train for object detection 
```
cd ROOT_MaskYolo

# prepare voc labels, set the categories you want to detect in scripts/voc_label.py
python scripts/voc_label.py --voc_dir=/path/to/voc_dir/

# generate lmdb for detection
sh ./scripts/convert_detection.sh  /path/to/train.txt /path/to/lmdb   

# train the detection model
cd ./models/mb_v2_t4_cls5_yolo/
sh train_yolo.sh
```

### Instance Mask and Keypoints

#### instance mask and keypoints demo
```
cd tools
python mask_inference.py [--img_path=xxx.jpg] [--model=xxx.prototxt] [--weights=xxx.caffemodel] 
# Net forward time consumed: 8.67ms

python kps_inference.py [--img_path=xxx.jpg] [--model=xxx.prototxt] [--weights=xxx.caffemodel] 
# Net forward time consumed: 5.58ms
```

some resulting samples are show below. 
I just feed yolo results to `roi_pooing` or `roi_alignment` layer.

![](assets/mask_keypoints.png)

#### train for mask regression
```
# compile the pythonapi of cocoapi
cd ROOT_MaskYolo/lib/cocoapi/PythonAPI
make -j

# use the following command to generate lmdb which contains mask and keypoints information
cd ROOT_MaskYolo
python scripts/createdata_mask_kps.py --coco_dir=/path/to/coco --lmdb_dir=/path/to/lmdb

# the training for mask consists of 2 steps 
cd ./models/mb_body_mask

# 1. freeze the weights of detection network, only update the roi mask part
sh train_maskyolo_step1.sh

# 2. update all the network with finetuning the model of step1
sh train_maskyolo_step2.sh

```



### Reference

> You Only Look Once: Unified, Real-Time Object detection http://arxiv.org/abs/1506.02640

> YOLO9000: Better, Faster, Stronger https://arxiv.org/abs/1612.08242

> YOLOv3: An Incremental Improvement https://pjreddie.com/media/files/papers/YOLOv3.pdf

> Faster R-CNN: Towards Real-Time Object Detection with Region Proposal Networks

> Mask R-CNN 

