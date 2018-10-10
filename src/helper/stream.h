
using namespace iod;
using namespace vpp;
using namespace s;

decltype(auto) operator>>(clc_str const &path, cimarron::pre::preprocessing poc)
    -> framevector {
  return poc.prepare(path);
}

decltype(auto) operator>>(framevector const &f,
                          cimarron::analysis::analysis anal)
    -> framesAndGlobalMotionData {
  std::cout << f.size() << std::endl;
  // frames in fandgdd is not const since we need to transform it in the next
  // step
  return framesAndGlobalMotionData{f, anal.analyze(f)};
}

decltype(auto) operator>>(framesAndGlobalMotionData const &fgdd,
                          cimarron::stabilziation::stabilziation stab)
    -> framevector {
  std::cout << fgdd.frames.size() << std::endl;
  return stab.stabilze(fgdd.frames, fgdd.gdd);
}

decltype(auto) operator>>(framevector const &f, cimarron::post::video_output vo)
    -> cimarron::post::video_output {
  std::cout << "operator vo: " << (int)f[0].domain().ncols() << std::endl;
  vo.set(f);
  return std::move(vo);
}
