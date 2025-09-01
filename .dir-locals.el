((c-mode . ((compile-command . "cd ..; make -B test")
            (dape-configs . ((gdb-test-unit
                              modes (c-mode)
                              command-cwd dape-command-cwd
                              command "/usr/local/bin/gdb"
                              command-args ("--interpreter=dap")
                              compile "make -B build/unit DEBUG=1"
                              :request "launch"
                              :program "build/unit")
                             (gdb-5raysphere
                              modes (c-mode)
                              command-cwd dape-command-cwd
                              command "/usr/local/bin/gdb"
                              command-args ("--interpreter=dap")
                              compile "make -B build/5raysphere DEBUG=1"
                              :request "launch"
                              :program "build/5raysphere")
                             (gdb-10pattern
                              modes (c-mode)
                              command-cwd dape-command-cwd
                              command "/usr/local/bin/gdb"
                              command-args ("--interpreter=dap")
                              compile "make -B build/10pattern DEBUG=1 DIMENSION=100"
                              :request "launch"
                              :program "build/10pattern"))))))
