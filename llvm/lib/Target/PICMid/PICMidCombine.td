// See https://llvm.org/docs/GlobalISel/MIRPatterns.html
def select_frame_index : GICombineRule<
    (defs root:$dst),
    (match (G_FRAME_INDEX $dst, $stack_addr)),
    (apply (G_CONSTANT $dst, $stack_addr))
>;

def PICMidFrameIndexCombiner : GICombiner<"PICMidFrameIndexCombinerImpl", [
    select_frame_index,
]>;