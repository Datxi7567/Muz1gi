local player = game.Players.LocalPlayer
local gui = Instance.new("ScreenGui")
gui.Name = "CustomSeedStock"
gui.ResetOnSpawn = false
gui.Parent = player:WaitForChild("PlayerGui")

local mainFrame = Instance.new("Frame")
mainFrame.Size = UDim2.new(0, 350, 0, 400)
mainFrame.Position = UDim2.new(0.5, -175, 0.5, -200)
mainFrame.BackgroundColor3 = Color3.fromRGB(25, 25, 25)
mainFrame.BackgroundTransparency = 0.2
mainFrame.Parent = gui

local title = Instance.new("TextLabel")
title.Size = UDim2.new(1, 0, 0, 30)
title.BackgroundTransparency = 1
title.Text = "Danh sách hạt giống đang Stock"
title.TextColor3 = Color3.fromRGB(255, 255, 255)
title.Font = Enum.Font.SourceSansBold
title.TextSize = 18
title.Parent = mainFrame

local scroll = Instance.new("ScrollingFrame")
scroll.Size = UDim2.new(1, -10, 1, -40)
scroll.Position = UDim2.new(0, 5, 0, 35)
scroll.CanvasSize = UDim2.new(0, 0, 0, 0)
scroll.ScrollBarThickness = 6
scroll.BackgroundTransparency = 1
scroll.Parent = mainFrame

-- Hàm quét GUI shop và update GUI riêng
local function RefreshStock()
    scroll:ClearAllChildren()

    local shopGui = player:WaitForChild("PlayerGui"):FindFirstChild("Seed_Shop")
    if not shopGui then return end

    local listFrame = shopGui:FindFirstChildWhichIsA("ScrollingFrame", true)
    if not listFrame then return end

    local yPos = 0
    for _, frame in ipairs(listFrame:GetChildren()) do
        if frame:IsA("Frame") or frame:IsA("TextButton") then
            local seedName, price, stock, icon

            for _, obj in ipairs(frame:GetDescendants()) do
                if obj:IsA("TextLabel") then
                    if obj.Name == "Seed_Text" then
                        seedName = obj.Text
                    elseif obj.Name == "Cost_Text" then
                        price = obj.Text
                    elseif obj.Name == "Stock_Text" then
                        stock = obj.Text
                    end
                elseif obj:IsA("ImageLabel") or obj:IsA("ImageButton") then
                    if obj.Image and obj.Image ~= "" and not obj.Image:find("rbxasset://") then
                        icon = obj.Image
                    end
                end
            end

            if seedName and price and stock and (not stock:find("X0")) and (not price:find("NO STOCK")) then
                local itemFrame = Instance.new("Frame")
                itemFrame.Size = UDim2.new(1, -10, 0, 50)
                itemFrame.Position = UDim2.new(0, 5, 0, yPos)
                itemFrame.BackgroundTransparency = 1
                itemFrame.Parent = scroll

                local img = Instance.new("ImageLabel")
                img.Size = UDim2.new(0, 40, 0, 40)
                img.Position = UDim2.new(0, 0, 0.5, -20)
                img.BackgroundTransparency = 1
                img.Image = icon
                img.Parent = itemFrame

                local text = Instance.new("TextLabel")
                text.Size = UDim2.new(1, -50, 1, 0)
                text.Position = UDim2.new(0, 50, 0, 0)
                text.BackgroundTransparency = 1
                text.TextXAlignment = Enum.TextXAlignment.Left
                text.Text = seedName .. " | Giá: " .. price .. " | Stock: " .. stock
                text.TextColor3 = Color3.fromRGB(255, 255, 255)
                text.Font = Enum.Font.SourceSans
                text.TextSize = 16
                text.Parent = itemFrame

                yPos = yPos + 55
            end
        end
    end
    scroll.CanvasSize = UDim2.new(0, 0, 0, yPos)
end

-- Refresh ngay khi chạy và mỗi 10s
RefreshStock()
while task.wait(10) do
    RefreshStock()
end
