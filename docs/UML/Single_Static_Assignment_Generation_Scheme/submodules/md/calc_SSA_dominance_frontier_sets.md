@startuml
partition calc_SSA_dominance_frontier_sets {
    start
    fork
        :CFGNode *t_inp_post_order_CFG <- INPUT;
    fork again
        :Size_tLL *t_inp_pred_arr <- INPUT;
    fork again
        :size_t *t_inp_doms_arr <- INPUT;
    fork again
        :size_t t_inp_size <- INPUT;
    fork end
    fork
        :size_t t_c <- 0;
    fork again
        :size_t t_sc <- 0;
    fork again
        :Size_tLL t_df[t_inp_size];
    fork end
    while (t_c < t_inp_size)
        :size_t t_num_preds <- calc_SSA_num_preds
        (t_inp_pred_arr, t_c);
        if (t_num_preds > 1) then (TRUE)
            while (t_sc < t_num_preds)
                :size_t t_runner <- calc_SSA_nth_predecessor
                (t_inp_pred_arr[t_c], t_sc);
                while (t_runner != t_inp_doms_arr[t_c])
                    :t_df[t_runner] <- calc_SSA_append_to_df
                    (t_df[t_runner], t_c);
                    :t_runner <- t_inp_doms[t_runner];
                endwhile
                :t_sc <- t_sc + 1;
            endwhile
        endif
    :t_c <- t_c + 1;
    endwhile
    :OUTPUT t_df;
    stop
}
@enduml
