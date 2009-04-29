require "lmkbuild"

local ipairs = ipairs
local pairs = pairs
local print = print
local sys = lmkbuild.system ()
local resolve = lmkbuild.resolve
local lmk = lmk

module (...)

function get_flags (flags) return lmk.merge_tables ({DMZ_USE_V8 = true, }, flags) end

function set_name (name, flags)
   lmk.set_name (name, get_flags (flags))
end

function add_libs ()
   local libDebug = ""
   if sys == "win32" then
      local buildMode = resolve ("$(lmk.buildMode)")
      if buildMode == "debug" then libDebug = resolve ("$(DMZ_V8_DEBUG)") end
   end
   local includePath = resolve ("$(DMZ_V8_INCLUDE_PATH)")
   if not includePath or includePath == "" then
      includePath = "$(lmk.projectRoot)depend/v8/include"
   end
   local libPath = resolve ("$(DMZ_V8_LIB_PATH)")
   if not libPath or libPath == "" then
      libPath = "$(lmk.projectRoot)depend/v8/lib"
   end
   local lib = "$(lmk.libPrefix)" .. "v8" .. libDebug .. "$(lmk.libSuffix)"
   lmk.add_vars {
      localIncludes = "$(lmk.includePathFlag)" .. includePath,
      localLibPaths = "$(lmk.libPathFlag)" .. libPath,
      localLibs = lib,
   }
end
