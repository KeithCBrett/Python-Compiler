@startuml
    partition calc_CFG {
        start
        :VasmInst *inp_vasm = INPUT;
        fork
            partition calc_CFG_num_blocks {
                :VasmInstr *t_inp_vasm = inp_vasm;
                :t_out = 0;
                while (t_inp_vasm != NULL)
                    if (t_inp_vasm is head of list) then (TRUE)
                        fork
                            :t_out++;
                        fork again
                            :t_inp_vasm = t_inp_vasm->next;
                        fork end
                    elseif (t_inp_vasm->type == label) then (TRUE)
                        fork
                            :t_out++;
                        fork again
                            :t_inp_vasm = t_inp_vasm->next;
                        fork end
                    elseif (t_inp_vasm->type == jle) then (TRUE)
                        fork
                            :t_out++;
                        fork again
                            :t_inp_vasm = t_inp_vasm->next;
                        fork end
                    else (FALSE)
                        :t_inp_vasm = t_inp_vasm->next;
                    endif
                endwhile
                :s = t_out;
            }
        fork again
            partition calc_CFG_head_arr {
                :VasmInstr t_inp_vasm = inp_vasm;
                fork
                    partition calc_CFG_num_blocks {
                        :VasmInstr *tt_inp_vasm = t_inp_vasm;
                        :tt_out = 0;
                        while (tt_inp_vasm != NULL)
                            if (tt_inp_vasm is head of list) then (TRUE)
                                fork
                                    :tt_out++;
                                fork again
                                    :tt_inp_vasm = tt_inp_vasm->next;
                                fork end
                            elseif (tt_inp_vasm->type == label) then (TRUE)
                                fork
                                    :tt_out++;
                                fork again
                                    :tt_inp_vasm = tt_inp_vasm->next;
                                fork end
                            elseif (tt_inp_vasm->type == jle) then (TRUE)
                                fork
                                    :tt_out++;
                                fork again
                                    :tt_inp_vasm = tt_inp_vasm->next;
                                fork end
                            else (FALSE)
                                :tt_inp_vasm = tt_inp_vasm->next;
                            endif
                        endwhile
                        :t_s = tt_out;
                    }
                fork again
                    :t_c = 0;
                fork end
                :t_out[t_s];
                while (t_inp_vasm != NULL)
                    if (t_inp_vasm is head of list) then (TRUE)
                        :t_out = t_c;
                        fork
                            :t_out++;
                        fork again
                            :t_inp_vasm = t_inp_vasm->next;
                        fork end
                    elseif (t_inp_vasm->type == label) then (TRUE)
                        :t_out = t_c;
                        fork
                            :t_out++;
                        fork again
                            :t_inp_vasm = t_inp_vasm->next;
                        fork end
                    elseif (t_inp_vasm->type == jle) then (TRUE)
                        fork
                            :t_inp_vasm = t_inp_vasm->next;
                        fork again
                            :t_c++;
                        fork end
                        :t_out = t_c;
                        :t_out++;
                    else (FALSE)
                    endif
                endwhile
                :h = t_out;
            }
        fork again
            partition calc_CFG_tail_arr {
                :VasmInstr *t_inp_vasm = inp_vasm;
                fork
                    partition calc_CFG_head_arr {
                        :VasmInstr tt_inp_vasm = t_inp_vasm;
                        fork
                            partition calc_CFG_num_blocks {
                                :VasmInstr *ttt_inp_vasm = tt_inp_vasm;
                                :ttt_out = 0;
                                while (ttt_inp_vasm != NULL)
                                    if (ttt_inp_vasm is head of list) then (TRUE)
                                        fork
                                            :ttt_out++;
                                        fork again
                                            :ttt_inp_vasm = ttt_inp_vasm->next;
                                        fork end
                                    elseif (ttt_inp_vasm->type == label) then (TRUE)
                                        fork
                                            :ttt_out++;
                                        fork again
                                            :ttt_inp_vasm = ttt_inp_vasm->next;
                                        fork end
                                    elseif (ttt_inp_vasm->type == jle) then (TRUE)
                                        fork
                                            :ttt_out++;
                                        fork again
                                            :ttt_inp_vasm = ttt_inp_vasm->next;
                                        fork end
                                    else (FALSE)
                                        :ttt_inp_vasm = ttt_inp_vasm->next;
                                    endif
                                endwhile
                                :tt_s = ttt_out;
                            }
                        fork again
                            :tt_c = 0;
                        fork end
                        :tt_out[tt_s];
                        while (tt_inp_vasm != NULL)
                            if (tt_inp_vasm is head of list) then (TRUE)
                                :tt_out = tt_c;
                                fork
                                    :tt_out++;
                                fork again
                                    :tt_inp_vasm = tt_inp_vasm->next;
                                fork end
                            elseif (tt_inp_vasm->type == label) then (TRUE)
                                :tt_out = tt_c;
                                fork
                                    :tt_out++;
                                fork again
                                    :tt_inp_vasm = tt_inp_vasm->next;
                                fork end
                            elseif (tt_inp_vasm->type == jle) then (TRUE)
                                fork
                                    :tt_inp_vasm = tt_inp_vasm->next;
                                fork again
                                    :tt_c++;
                                fork end
                                :tt_out = tt_c;
                                :tt_out++;
                            else (FALSE)
                            endif
                        endwhile
                        :t_h = tt_out;
                    }
                fork again
                    :t_c = 0;
                fork again
                    partition calc_CFG_num_blocks {
                        :VasmInstr *tt_inp_vasm = t_inp_vasm;
                        :tt_out = 0;
                        while (tt_inp_vasm != NULL)
                            if (tt_inp_vasm is head of list) then (TRUE)
                                fork
                                    :tt_out++;
                                fork again
                                    :tt_inp_vasm = tt_inp_vasm->next;
                                fork end
                            elseif (tt_inp_vasm->type == label) then (TRUE)
                                fork
                                    :tt_out++;
                                fork again
                                    :tt_inp_vasm = tt_inp_vasm->next;
                                fork end
                            elseif (tt_inp_vasm->type == jle) then (TRUE)
                                fork
                                    :tt_out++;
                                fork again
                                    :tt_inp_vasm = tt_inp_vasm->next;
                                fork end
                            else (FALSE)
                                :tt_inp_vasm = tt_inp_vasm->next;
                            endif
                        endwhile
                        :t_s = tt_out;
                    }
                fork end
                    :t_out[t_s];
                while (t_c != (t_s-1))
                    :t_out[t_c] = t_h[t_c+1]) - 1;
                    :t_c++;
                endwhile
                partition calc_CFG_num_list_items {
                    fork
                        :VasmInstr *tt_inp_vasm = t_inp_vasm;
                    fork again
                        :tt_out = 0;
                    fork end
                    while (tt_inp_vasm->next != NULL)
                        fork
                            :tt_out++;
                        fork again
                            :tt_inp_vasm = tt_inp_vasm->next;
                        fork end
                    endwhile
                    :t_out[t_s] = tt_out;
                }
                :t = t_out;
            }
        fork again
            :c = 0;
        fork end
        :out[s];
        while (c < s)
            fork
                partition calc_CFG_inst {
                    fork
                        :VasmInstr *t_inp_vasm = inp_vasm;
                    fork again
                        :size_t t_inp_index = h[c];
                    fork end
                    fork
                        :t_out = *t_inp_vasm;
                    fork again
                        :t_c = 0;
                    fork end
                    while (t_c != t_inp_index)
                        fork
                            :t_out = t_out->next;
                        fork again
                            :t_c++;
                        fork end
                    endwhile
                    :xh = t_out;
                }
            fork again
                partition calc_CFG_inst {
                    fork
                        :VasmInstr *t_inp_vasm = inp_vasm;
                    fork again
                        :size_t t_inp_index = t[c];
                    fork end
                    fork
                        :t_out = *t_inp_vasm;
                    fork again
                        :t_c = 0;
                    fork end
                    while (t_c != t_inp_index)
                        fork
                            :t_out = t_out->next;
                        fork again
                            :t_c++;
                        fork end
                    endwhile
                    :xt = t_out;
                }
            fork end
            partition calc_CFG_blocks {
                fork
                    :VasmInst t_head = xh;
                fork again
                    :VasmInst t_tail = xt;
                fork end
                fork
                    :VasmInst *t_out = &t_head;
                fork again
                    :VasmInst *t_temp = &t_head;
                fork end
                while (t_temp != t_tail)
                    :t_temp = t_temp->next;
                endwhile
                :t_temp->next = NULL;
                :out[c]->contents = t_out;
            }
            :c++;
        endwhile
        partition calc_CFG_edges {
            fork
                :CFGNode *t_inp_node = out;
            fork again
                :size_t t_inp_size = s;
            end fork
            fork
                :t_c = 0;
            fork again
                :t_out = t_inp_node;
            end fork
            :t_t = t_out;
            while (t_c != t_inp_size)
                partition calc_CFG_last_inst {
                    :VasmInst *tt_inp_vasm = t_t->contents;
                    :tt_out = tt_inp_vasm;
                    while (tt_out->next != NULL)
                        :tt_out = tt_out->next;
                    endwhile
                    :t_l = tt_out;
                }
                partition calc_CFG_loop_const {
                    fork
                        :CFGNode *tt_inp_out = t_inp_node;
                    fork again
                        :size_t tt_inp_index = t_l->left;
                    fork end
                    while ((tt_inp_out != label) && (tt_inp_out->left != tt_inp_index))
                        :tt_inp_out = tt_inp_out->next;
                    endwhile
                    :t_LC = tt_inp_out;
                }
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
            :out = t_out;
        }
        :OUTPUT out;
        stop
    }
@enduml
