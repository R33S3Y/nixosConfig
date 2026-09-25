-- ${NIX.type.thing} is replaced by nix, when it reads the file.

hl.bind("SUPER + Q", hl.dsp.exec_cmd("kitty"))
hl.bind("SUPER + W", hl.dsp.exec_cmd("pcmanfm"))
hl.bind("SUPER + Z", hl.dsp.exec_cmd('rofi -modes "run,ssh" -show drun'))
hl.bind("SUPER + S", hl.dsp.exec_cmd('firefox'))

hl.bind("SUPER + A", hl.dsp.window.float({ action = "toggle" }))
hl.bind("SUPER + X", hl.dsp.window.close())

-- Move focus with mainMod + arrow keys
hl.bind("SUPER + left", hl.dsp.focus({ direction = "left" }))
hl.bind("SUPER + right", hl.dsp.focus({ direction = "right" }))
hl.bind("SUPER + up", hl.dsp.focus({ direction = "up" }))
hl.bind("SUPER + down", hl.dsp.focus({ direction = "down" }))

-- Switch workspaces with mainMod + [0-9]
-- Move active window to a workspace with mainMod + SHIFT + [0-9]
for i = 1, 10 do
  local key = i % 10 -- 10 maps to key 0
  hl.bind("SUPER + " .. key, hl.dsp.focus({ workspace = i }))
  hl.bind("SUPER + SHIFT + " .. key, hl.dsp.window.move({ workspace = i }))
end

-- Scroll through existing workspaces with mainMod + scroll
hl.bind("SUPER + mouse_down", hl.dsp.focus({ workspace = "r+1" }))
hl.bind("SUPER + mouse_up", hl.dsp.focus({ workspace = "r-1" }))

hl.bind("SUPER + SHIFT + mouse_down", hl.dsp.window.move({ workspace = "r+1" }))
hl.bind("SUPER + SHIFT + mouse_up", hl.dsp.window.move({ workspace = "r-1" }))
hl.bind("SUPER + SHIFT + mouse_down", hl.dsp.focus({ workspace = "r+1" }))
hl.bind("SUPER + SHIFT + mouse_up", hl.dsp.focus({ workspace = "r-1" }))

-- Screenshot a window
hl.bind("SUPER + SHIFT + PRINT",
  hl.dsp.exec_cmd("hyprshot -m window -o ${NIX.user.screenshotFolder}"))

-- Screenshot a region
hl.bind("PRINT", hl.dsp.exec_cmd("hyprshot -m region -o ${NIX.user.screenshotFolder}"))

-- Screenshot a monitor
hl.bind("SUPER + PRINT", hl.dsp.exec_cmd("hyprshot -m output -o ${NIX.user.screenshotFolder}"))

-- color picker
hl.bind("SUPER + CTRL + PRINT", hl.dsp.exec_cmd("hyprpicker -a"))
-- Move/resize windows with mainMod + LMB/RMB and dragging
hl.bind("SUPER + mouse:272", hl.dsp.window.drag(), { mouse = true })
hl.bind("SUPER + mouse:273", hl.dsp.window.resize(), { mouse = true })
