local player = game.Players.LocalPlayer
local shopGui = player:WaitForChild("PlayerGui"):WaitForChild("Seed_Shop")
local listFrame = shopGui:FindFirstChildWhichIsA("ScrollingFrame", true)

-- GUI riêng
local screenGui = Instance.new("ScreenGui")
screenGui.Name = "StockViewer"
screenGui.Parent = player.PlayerGui

local mainFrame = Instance.new("Frame")
mainFrame.Size = UDim2.new(0, 360, 0, 420)
mainFrame.Position = UDim2.new(0, 20, 0.5, -200)
mainFrame.BackgroundColor3 = Color3.fromRGB(25, 25, 25)
mainFrame.BackgroundTransparency = 0.2
mainFrame.BorderSizePixel = 0
mainFrame.Parent = screenGui

local uiList = Instance.new("UIListLayout")
uiList.Parent = mainFrame
uiList.Padding = UDim.new(0, 6)

local countdownLabel = Instance.new("TextLabel")
countdownLabel.Size = UDim2.new(1, -10, 0, 25)
countdownLabel.BackgroundTransparency = 1
countdownLabel.TextColor3 = Color3.fromRGB(0, 255, 0)
countdownLabel.Font = Enum.Font.SourceSansBold
countdownLabel.TextSize = 18
countdownLabel.TextXAlignment = Enum.TextXAlignment.Left
countdownLabel.Parent = mainFrame

-- Thử require module dữ liệu seeds
local SeedsData
pcall(function()
	SeedsData = require(game.ReplicatedStorage.Data.SeedsData)
end)

local function clearFrame()
	for _, obj in ipairs(mainFrame:GetChildren()) do
		if obj:IsA("TextLabel") or obj:IsA("Frame") then
			if obj ~= countdownLabel then
				obj:Destroy()
			end
		end
	end
end

local function addItemLine(seedName, price, stock, imageId)
	local itemFrame = Instance.new("Frame")
	itemFrame.Size = UDim2.new(1, -10, 0, 40)
	itemFrame.BackgroundTransparency = 1
	itemFrame.Parent = mainFrame

	local icon = Instance.new("ImageLabel")
	icon.Size = UDim2.new(0, 32, 0, 32)
	icon.Position = UDim2.new(0, 0, 0, 4)
	icon.BackgroundTransparency = 1
	icon.Image = imageId or "rbxassetid://0"
	icon.Parent = itemFrame

	local txt = Instance.new("TextLabel")
	txt.Size = UDim2.new(1, -40, 1, 0)
	txt.Position = UDim2.new(0, 40, 0, 0)
	txt.BackgroundTransparency = 1
	txt.TextColor3 = Color3.fromRGB(255,255,0)
	txt.Font = Enum.Font.SourceSans
	txt.TextSize = 16
	txt.TextXAlignment = Enum.TextXAlignment.Left
	txt.Text = seedName .. " | Giá: " .. price .. " | Stock: " .. stock
	txt.Parent = itemFrame
end

local function refreshStock()
	clearFrame()
	countdownLabel.Text = "Đang cập nhật..."

	for _, frame in ipairs(listFrame:GetChildren()) do
		if frame:IsA("Frame") or frame:IsA("TextButton") then
			local seedName, price, stock

			for _, obj in ipairs(frame:GetDescendants()) do
				if obj:IsA("TextLabel") then
					if obj.Name == "Seed_Text" then
						seedName = obj.Text
					elseif obj.Name == "Cost_Text" then
						price = obj.Text
					elseif obj.Name == "Stock_Text" then
						stock = obj.Text
					end
				end
			end

			if seedName and price and stock then
				if not stock:find("X0") and not price:find("NO STOCK") then
					local imageId

					-- Nếu có module SeedsData thì thử tra icon
					if SeedsData and SeedsData[seedName] and SeedsData[seedName].IconId then
						imageId = "rbxassetid://".. tostring(SeedsData[seedName].IconId)
					end

					addItemLine(seedName, price, stock, imageId)
				end
			end
		end
	end

	countdownLabel.Text = "Danh sách hạt giống (cập nhật 10s)"
end

-- Loop update
task.spawn(function()
	while true do
		refreshStock()
		task.wait(10)
	end
end)

refreshStock()
