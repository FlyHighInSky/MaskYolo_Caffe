#ifndef CAFFE_KPS_BOX_DATA_LAYER_HPP_
#define CAFFE_KPS_BOX_DATA_LAYER_HPP_

#include <vector>

#include "caffe/blob.hpp"
#include "caffe/data_transformer.hpp"
#include "caffe/internal_thread.hpp"
#include "caffe/layer.hpp"
#include "caffe/layers/base_data_layer.hpp"
#include "caffe/proto/caffe.pb.h"
#include "caffe/util/db.hpp"

namespace caffe {

template <typename Dtype>
class KpsBoxDataLayer : public BasePrefetchingDataLayer<Dtype> {
 public:
  explicit KpsBoxDataLayer(const LayerParameter& param);
  virtual ~KpsBoxDataLayer();
  virtual void DataLayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual inline const char* type() const { return "BoxData"; }
  virtual inline int ExactNumBottomBlobs() const { return 0; }
  virtual inline int MinTopBlobs() const { return 1; }
  virtual inline int MaxTopBlobs() const { return 2; }

  void transform_label_v2(Dtype* top_label, const vector<BoxLabel>& box_labels);
  void transform_label_mask(Dtype* top_label, const vector<BoxLabel>& box_labels, const cv::Mat& mask);
  void transform_label_kps(Dtype* top_label, const vector<BoxLabel>& box_labels, const vector<KpsLabel>& kps_labels, const cv::Mat& mask);

 protected:
  void Next();
  bool Skip();
  virtual void load_batch(Batch<Dtype>* batch);

  shared_ptr<db::DB> db_;
  shared_ptr<db::Cursor> cursor_;
  uint64_t offset_;

  float min_size_and;
  float min_size_or;
  int max_box_num;
};

}  // namespace caffe

#endif  // CAFFE_KPS_BOX_DATA_LAYER_HPP_
