@startuml
partition calc_CFG_edges {
start
    fork
        :CFGNode *t_inp_node = INPUT;
    fork again
        :size_t t_inp_size = INPUT;
    end fork
    fork
        :t_c = 0;
    fork again
        :t_out = t_inp_node;
    end fork
    :t_t = t_out;
    while (t_c != t_inp_size)
        :t_l = calc_CFG_last_inst(t_t->contents);
        :t_LC = calc_CFG_loop_const(t_l->left, t_inp_node);
        if (t_l->type = jmp) then (TRUE)
            :t_t->left = t_LC;
            :t_c++;
            :t_t++;
        elseif (t_l->type = jle) then (TRUE)
            :t_t->right = t_LC;
            :t_t->left = t_inp_node[c+1];
            :t_c++;
            :t_t++;
        else (FALSE)
            :t_c++;
            :t_t++;
        endif
    endwhile
    :OUTPUT t_out;
stop
}
@enduml
