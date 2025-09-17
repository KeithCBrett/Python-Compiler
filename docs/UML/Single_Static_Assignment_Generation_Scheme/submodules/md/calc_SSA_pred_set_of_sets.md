@startuml
partition calc_SSA_pred_set_of_sets {
    start
    fork
    :CFGNode *t_post_order_CFG <- INPUT;
    fork again
    :size_t t_size <- INPUT;
    fork end
    fork
    :Size_tLL t_out[t_size];
    fork again
    :size_t t_c <- 0;
    fork end
    while (t_c < t_size)
        :CFGNode *t_curr <- t_post_order_CFG[t_c];
        :size_t t_i_curr <- calc_SSA_CFG_post_order_index
        (t_post_order_CFG, t_curr);
        if (t_curr.left != NULL) then (TRUE)
            :size_t t_node_index <- calc_SSA_CFG_post_order_index
            (t_post_order_CFG, t_curr.left);
            :t_out[t_node_index] <- calc_SSA_append_to_pred_list
            (t_out[t_node_index], t_i_curr);
        if (t_curr.right != NULL) then (TRUE)
            :size_t t_node_index <- calc_SSA_CFG_post_order_index
            (t_post_order_CFG, t_curr.right);
            :t_out[t_node_index] <- calc_SSA_append_to_pred_list
            (t_out[t_node_index], t_i_curr);
        endif
        endif
        :t_c <- t_c + 1;
    endwhile
    :OUTPUT t_out;
    stop
}
@enduml
