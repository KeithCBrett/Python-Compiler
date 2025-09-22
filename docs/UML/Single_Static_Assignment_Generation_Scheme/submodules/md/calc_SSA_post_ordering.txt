@startuml
    partition calc_SSA_DFS {
        start
        fork
        :size_t t_size <- INPUT;
        fork again
        :CFGNode *t_cfg <- INPUT;
        fork end
        fork
        :size_t t_c <- 0;
        fork again
        :CFGNode t_out[t_size];
        fork again
        :CFGNode *t_stack <- NULL;
        fork end
        while (t_c < t_size)
            :t_cfg[t_c].color <- w;
            :t_c <- t_c + 1;
        endwhile
        fork
        :t_c <- 0;
        fork again
        :t_cfg.color <- b;
        fork again
        :t_stack <- push(t_stack, t_cfg);
        fork end
        :t_out[t_c] <- t_cfg;
        :t_c <- t_c + 1;
        while (t_stack != NULL)
            if ((t_cfg.left != NULL) && (t_cfg.left.color != b)) then (TRUE)
                :t_cfg <- t_cfg.left;
                fork
                :t_cfg.color <- b;
                fork again
                :t_out[t_c] <- t_cfg;
                fork again
                :t_stack <- push(t_stack, t_cfg);
                fork end
                :t_c <- t_c + 1;
            elseif ((t_cfg.right != NULL) && (t_cfg.right.color != b)) then (TRUE)
                :t_cfg <- t_cfg.right;
                fork
                :t_cfg.color <- b;
                fork again
                :t_out[t_c] <- t_cfg;
                fork again
                :t_stack <- push(t_stack, t_cfg);
                fork end
                :t_c <- t_c + 1;
            else (FALSE)
                :t_cfg <- pop(t_stack);
            endif
        endwhile
        :OUTPUT t_out;
        stop
    }
@enduml
