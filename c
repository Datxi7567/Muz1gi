local player = game.Players.LocalPlayer
local shopGui = player:WaitForChild("PlayerGui"):WaitForChild("Seed_Shop")
local listFrame = shopGui:FindFirstChildWhichIsA("ScrollingFrame", true)

-- GUI riêng
local screenGui = Instance.new("ScreenGui")
screenGui.Name = "StockViewer"
screenGui.Parent = player.PlayerGui

local mainFrame = Instance.new("Frame")
mainFrame.Size = UDim2.new(0.35, 0, 0.45, 0) -- chiếm 35% ngang, 45% cao
mainFrame.Position = UDim2.new(0.02, 0, 0.15, 0) -- cách trái 2%, cách trên 15%
mainFrame.BackgroundColor3 = Color3.fromRGB(25, 25, 25)
mainFrame.BackgroundTransparency = 0.2
mainFrame.BorderSizePixel = 0
mainFrame.Parent = screenGui

local uiList = Instance.new("UIListLayout")
uiList.Parent = mainFrame
uiList.Padding = UDim.new(0, 6)

-- Countdown label
local countdownLabel = Instance.new("TextLabel")
countdownLabel.Size = UDim2.new(1, -10, 0, 0.08) -- cao 8% frame
countdownLabel.BackgroundTransparency = 1
countdownLabel.TextColor3 = Color3.fromRGB(0, 255, 0)
countdownLabel.Font = Enum.Font.SourceSansBold
countdownLabel.TextScaled = true
countdownLabel.TextXAlignment = Enum.TextXAlignment.Left
countdownLabel.Parent = mainFrame

local function clearFrame()
	for _, obj in ipairs(mainFrame:GetChildren()) do
		if (obj:IsA("TextLabel") or obj:IsA("Frame")) and obj ~= countdownLabel then
			obj:Destroy()
		end
	end
end

-- Lấy countdown từ GUI shop
local function getCountdownText()
	local countdownLabelShop = shopGui:FindFirstChildWhichIsA("TextLabel", true)
	if countdownLabelShop and string.find(countdownLabelShop.Text, "New seeds") then
		return countdownLabelShop.Text
	end
	local ok, seedData = pcall(function()
		return require(game.ReplicatedStorage.Data.SeedShopData)
	end)
	if ok and type(seedData) == "table" and seedData.RefreshTime then
		return "Refresh in " .. tostring(seedData.RefreshTime) .. "s"
	end
	return "Countdown: N/A"
end

-- Map màu sắc theo rarity
local rarityColors = {
	["Common"] = Color3.fromRGB(200, 200, 200),
	["Uncommon"] = Color3.fromRGB(0, 255, 0),
	["Rare"] = Color3.fromRGB(0, 170, 255),
	["Epic"] = Color3.fromRGB(170, 0, 255),
	["Legendary"] = Color3.fromRGB(255, 215, 0),
}

-- Hàm thêm dòng có ảnh
local function addItemLine(seedName, price, stock, imageId, rarity)
	local itemFrame = Instance.new("Frame")
	itemFrame.Size = UDim2.new(1, -10, 0, 0.1 * mainFrame.AbsoluteSize.Y) -- 10% chiều cao frame
	itemFrame.BackgroundTransparency = 1
	itemFrame.Parent = mainFrame

	local icon = Instance.new("ImageLabel")
	icon.Size = UDim2.new(0.1, 0, 1, 0) -- icon chiếm 10% chiều ngang
	icon.BackgroundTransparency = 1
	icon.Image = imageId or "rbxassetid://0"
	icon.Parent = itemFrame

	local txt = Instance.new("TextLabel")
	txt.Size = UDim2.new(0.9, -5, 1, 0)
	txt.Position = UDim2.new(0.1, 5, 0, 0)
	txt.BackgroundTransparency = 1
	txt.TextColor3 = rarityColors[rarity] or Color3.fromRGB(255,255,255)
	txt.Font = Enum.Font.SourceSans
	txt.TextScaled = true
	txt.TextXAlignment = Enum.TextXAlignment.Left
	txt.Text = seedName .. " | Giá: " .. price .. " | Stock: " .. stock
	txt.Parent = itemFrame
end

-- Refresh stock
local function refreshStock()
	if not listFrame then
		warn("Không tìm thấy ScrollingFrame trong Seed_Shop")
		return
	end

	clearFrame()
	countdownLabel.Text = getCountdownText()

	for _, frame in ipairs(listFrame:GetChildren()) do
		if frame:IsA("Frame") or frame:IsA("TextButton") then
			local seedName, price, stock, shopIcon, rarityLabel

			seedName = frame:FindFirstChild("Seed_Text", true)
			price = frame:FindFirstChild("Cost_Text", true)
			stock = frame:FindFirstChild("Stock_Text", true)
			shopIcon = frame:FindFirstChild("ShopItem_Image", true)
			rarityLabel = frame:FindFirstChild("Rarity_Text", true)

			if seedName and price and stock then
				if not stock.Text:find("X0") and not price.Text:find("NO STOCK") then
					local imgId = shopIcon and shopIcon.Image or nil
					local rarity = rarityLabel and rarityLabel.Text or "Common"
					addItemLine(seedName.Text, price.Text, stock.Text, imgId, rarity)
				end
			end
		end
	end
end

-- Loop mỗi 1s
task.spawn(function()
	while true do
		refreshStock()
		task.wait(1)
	end
end)

refreshStock()
