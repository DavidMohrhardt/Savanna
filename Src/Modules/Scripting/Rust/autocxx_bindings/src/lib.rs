use autocxx::prelude::*;

include_cpp!(
    #include "SavannaCore_BindingsWrapper.h",
    safety!(unsafe_ffi)
    generate_ns!("Savanna")
);
