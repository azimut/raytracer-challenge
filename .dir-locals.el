((c-mode . ((dape-configs . ((gdb-test-unit
                              modes (c-mode)
                              command-cwd dape-command-cwd
                              command "gdb"
                              command-args ("--interpreter=dap")
                              compile "make -B test/unit DEBUG=1"
                              :request "launch"
                              :program "test/unit")
                             (gdb-test-5raysphere
                              modes (c-mode)
                              command-cwd dape-command-cwd
                              command "gdb"
                              command-args ("--interpreter=dap")
                              compile "make -B test/5raysphere DEBUG=1"
                              :request "launch"
                              :program "test/5raysphere"))))))
