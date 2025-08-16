((c-mode . ((dape-configs . ((gdb-test-unit
                              modes (c-mode)
                              command-cwd dape-command-cwd
                              command "gdb"
                              command-args ("--interpreter=dap")
                              :request "launch"
                              :program "test/unit"))))))
