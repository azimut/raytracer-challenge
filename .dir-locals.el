((c-mode . ((dape-configs . ((gdb-test-unit
                              modes (c-mode)
                              command-cwd dape-command-cwd
                              command "gdb"
                              command-args ("--interpreter=dap")
                              compile "make -B build/unit DEBUG=1"
                              :request "launch"
                              :program "build/unit")
                             (gdb-5raysphere
                              modes (c-mode)
                              command-cwd dape-command-cwd
                              command "gdb"
                              command-args ("--interpreter=dap")
                              compile "make -B build/5raysphere DEBUG=1"
                              :request "launch"
                              :program "build/5raysphere"))))))
