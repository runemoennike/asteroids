
inited = false;
myId = 0;

function init(myNewId)
	if(not inited) then
		myId = myNewId;
		inited = true;
		prn("Script initialization.");
	else
		prn("Warning: Initialized attempted multiple times. Ignoring.");
	end
end

function reset()
	if(inited) then
		prn("Script reset requested.");
		inited = false;
		init();
	else
		prn("Warning: Reset requested but script never inited. Ignoring.");
	end
end

function prn(text)
	print(myId, text)
end
	
	