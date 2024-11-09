def FeatureMidInstrs
    : SubtargetFeature<"mid-instrs", "HasMidInstrs", "true",
    "Original instructions present on all PIC(r) mid-range core devices">;

def FeatureEnhancedMidInstrs
    : SubtargetFeature<"enhanced-mid-instrs", "HasEnhancedMidInstrs", "false",
    "Additional instructions present in enhanced mid-range core devices">;

// A feature set aggregates features, grouping them. We don't want to create a
// new member in PICMidSubtarget (to store a value) for each set because we do not
// care if the set is supported, only the subfeatures inside the set. We fix
// this by simply setting the same dummy member for all feature sets, which is
// then ignored.
class FeatureSet<string name, string desc, list<SubtargetFeature> i>
  : SubtargetFeature<name, "FeatureSetDummy", "true", desc, i>;

// A family of microcontrollers, defining a set of supported features.
class Family<string name, string desc, list<SubtargetFeature> i>
  : FeatureSet<name, desc, i>;

def Family16F88x 
    : Family<
        "pic16f88x", 
        "Microchip's PIC16F882/883/884/886/887 mid-range family of microcontrollers", 
        [FeatureMidInstrs, FeatureStaticStack]>;

//===---------------------------------------------------------------------===//
// PIC Mid-range Families
//===---------------------------------------------------------------------===//

def PICMidSchedModel : SchedMachineModel {
  // Prevent instructions from being added to the pending queue, since latency
  // doesn't matter. The remaining latency concerns are disabled in the C++
  // scheduling strategy.
  let MicroOpBufferSize = 1;
  let CompleteModel = false;
}

class Device<string name, Family fam, list<SubtargetFeature> extraFeatures = []>
    : ProcessorModel<name, PICMidSchedModel, !listconcat([fam], extraFeatures)>;

def : Device<"pic16f88x", Family16F88x>;