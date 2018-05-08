#ifndef TEST_CEPH_TIME_H
#define TEST_CEPH_TIME_H

#include <list>

#include "include/encoding.h"
#include "common/ceph_time.h"
#include "common/Formatter.h"

// wrapper for ceph::real_time that implements the dencoder interface
template <typename Clock>
class time_point_wrapper {
  using time_point = typename Clock::time_point;
  time_point t;
 public:
  time_point_wrapper() = default;
  time_point_wrapper(const time_point& t) : t(t) {}

  void encode(bufferlist& bl) const {
    using ceph::encode;
    encode(t, bl);
  }
  void decode(bufferlist::iterator &p) {
    using ceph::decode;
    decode(t, p);
  }
  void dump(Formatter* f) {
    auto epoch_time = Clock::to_time_t(t);
    f->dump_string("time", std::ctime(&epoch_time));
  }
  static void generate_test_instances(std::list<time_point_wrapper*>& ls) {
    constexpr time_t t{455500800}; // Ghostbusters release date
    ls.push_back(new time_point_wrapper(Clock::from_time_t(t)));
  }
};

using real_time_wrapper = time_point_wrapper<ceph::real_clock>;
WRITE_CLASS_ENCODER(real_time_wrapper)

using coarse_real_time_wrapper = time_point_wrapper<ceph::coarse_real_clock>;
WRITE_CLASS_ENCODER(coarse_real_time_wrapper)

#endif
