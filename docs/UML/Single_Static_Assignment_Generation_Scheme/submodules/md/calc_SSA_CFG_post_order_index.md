@startuml
partition calc_SSA_CFG_post_order_index {
    start
    fork
        :CFGNode *t_post_order_arr <- INPUT;
    fork again
        :CFGNode *t_inp_node <- INPUT;
    fork end
    :size_t t_c <- 0;
    while (t_post_order_arr != t_inp_node)
        :t_c <- t_c + 1;
        :t_post_order_arr++;
    endwhile
    :OUTPUT t_c;
    stop
}
@enduml
