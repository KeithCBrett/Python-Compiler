@startuml
partition calc_SSA_operation_block_set {
    start
    fork
        :CFGNode *t_inp_CFG <- INPUT;
    fork again
        :size_t t_CFG_size <- INPUT;
    fork end
    :size_t t_set_size <- calc_SSA_block_set_num_vars(t_inp_CFG, t_CFG_size);
    ;size_t t_out[t_set_size];
    stop
}
@enduml
