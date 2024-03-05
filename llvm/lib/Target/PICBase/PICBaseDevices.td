def FeatureBase
    : SubtargetFeature<"base-instrs", "HasBaseInstrs", "true",
    "Original instructions present on all PIC(r) base-range core devices">;

// A feature set aggregates features, grouping them. We don't want to create a
// new member in PICBaseSubtarget (to store a value) for each set because we do not
// care if the set is supported, only the subfeatures inside the set. We fix
// this by simply setting the same dummy member for all feature sets, which is
// then ignored.
class FeatureSet<string name, string desc, list<SubtargetFeature> i>
  : SubtargetFeature<name, "FeatureSetDummy", "true", desc, i>;

// A family of microcontrollers, defining a set of supported features.
class Family<string name, string desc, list<SubtargetFeature> i>
  : FeatureSet<name, desc, i>;

def Family10F20x 
    : Family<
        "pic10f20x", 
        "Microchip's PIC10F200/202/204/206 baseline family of microcontrollers", 
        [FeatureBase, FeatureStaticStack]>;

//===---------------------------------------------------------------------===//
// PIC Base-range Families
//===---------------------------------------------------------------------===//

def PICBaseSchedModel : SchedMachineModel {
  // Prevent instructions from being added to the pending queue, since latency
  // doesn't matter. The remaining latency concerns are disabled in the C++
  // scheduling strategy.
  let MicroOpBufferSize = 1;
  let CompleteModel = false;
}

class Device<string name, Family fam, list<SubtargetFeature> extraFeatures = []>
    : ProcessorModel<name, PICBaseSchedModel, !listconcat([fam], extraFeatures)>;

def : Device<"pic10f200", Family10F20x>;