/******************************************************************************
 * Copyright (C) 2018-2023, idriverplus(BeiJing ZhiXingZhe, Inc.)
 *
 * History:
 * lbh          2019/03/28    1.0.0        build
 *****************************************************************************/

#include "common/base/log/include/log.h"

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace avos {
namespace common {

TEST(LogTest, LOG) {
  AvosNode::Init("test_node");
  for (int i = 0; i < 10; ++i) {
    AINFO << "ainfo:" << i;
    AWARN << "awarn:" << i;
    AERROR << "aerror:" << i;
    AINFO_IF(i > 5) << "ainfo_if:" << i;
    AWARN_IF(i > 5) << "awarn_if:" << i;
    AERROR_IF(i > 5) << "aerros_if:" << i;
    AINFO_EVERY(3) << "ainfo_every" << i;
    AWARN_EVERY(3) << "awarn_every" << i;
    AERROR_EVERY(3) << "aerror_every" << i;
  }
}

}  // namespace common
}  // namespace avos
