@startuml
partition calc_SSA_dominator_arr {
    start
    fork
    :CFGNode *t_post_order_CFG <- INPUT;
    fork again
    :size_t t_size <- INPUT;
    fork end
    fork
    :size_t t_doms[t_size];
    fork again
    :size_t t_c <- 0;
    fork again
    :PredLL *t_pred <- calc_SSA_pred_set_of_sets
    (t_post_order_CFG, t_size);
    fork end
    while (t_c < t_size)
        :t_doms[t_c] <- NULL;
        :t_c <- t_c + 1;
    endwhile
    fork
    :t_doms[0] <- 0;
    fork again
    :t_c <- 1;
    fork again
    :bool changed <- TRUE;
    fork again
    :size_t new_idom;
    fork end
    while (changed = TRUE)
        :changed <- FALSE;
        while (t_post_order_CFG[t_c] < t_size)
            :t_temp <- t_pred[t_c];
            :t_new_idom <- t_temp.contents;
            while (t_temp.next != NULL)
                :t_temp <- t_temp.next;
                if (t_doms[t_temp.contents] != NULL) then (TRUE)
                    :t_new_idom <- calc_SSA_dominator_intersect
                    (t_doms, t_temp.contents, t_new_idom);
                endif
            endwhile
            if (t_doms[t_c] != t_new_idom) then (FALSE)
                :t_doms[t_c] <- t_new_idom;
                :changed <- TRUE;
            endif
        endwhile
    endwhile
    :OUTPUT t_doms;
    stop
}
@enduml
