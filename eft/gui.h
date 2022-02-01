
extern RenderTexture* Texture;

namespace GUI {
	struct IO {
		Vector2 MousePosition;
		Vector2 PreviousMousePosition;
		Vector2 MouseDelta;
		USHORT CurrentElement;
	} IO;

	struct Window {
		bool Active = true;
		int TabIndex;

		bool WaitKey;

		Vector2 ParentSize;
		Vector2 Position;
		Vector2 Size;

		Vector2 TabsSize;
		Vector2 ContentSize;
	
		Vector2 TabPosition;
		Vector2 ItemPosition;
	} Window;

	struct Items {
		int Tab;
		int Checkbox;
		int Slider;
		int Input;
		int Select;
		int Bind;
		int ColorPicker;
	} Items;

	struct TabTemplate {
		wchar_t Name[64];
		Vector2 Position;
	};

	struct CheckboxTemplate {
		bool State;
		wchar_t Name[64];
		wchar_t Desc[64];
		ULONG DescColor;
		Vector2 Position;
	};
	
	struct SelectTemplate {
		wchar_t State[64];
		wchar_t Name[64];
		Vector2 Position;
	};

	struct SliderTemplate {
		float Value, Offset;
		wchar_t Name[64];
		wchar_t Desc[64];
		ULONG DescColor;
		Vector2 Position;
	};

	struct BindTemplate {
		wchar_t Name[64];
		wchar_t Key[64];
		Vector2 Position;
		bool KeyUp;
		bool KeyBind[256];
	};

	struct InputTemplate {
		wchar_t Name[64];
		wchar_t Desc[64];
		ULONG DescColor;
		wchar_t Value[64];
		Vector2 Position;
		bool Selected;
	};

	union ColorRGBA {
		ULONG Data;
		struct ChannelRGBA {
			BYTE B;
			BYTE G;
			BYTE R;
			BYTE A;
		} RGBA;
	};

	struct ColorPickerTemplate {
		wchar_t Name[64];
		Vector2 Position;
		ColorRGBA Color;
		bool Picked;
	};

	struct ListTemplate {
		bool IsVisible;
		bool Selected;
		wchar_t Label[64];
		wchar_t Placeholder[64];
		wchar_t Value[64];
		wchar_t Items[30][64];
		int ItemsCount;
		int DeleteIndex = -1;
	};

	TabTemplate TabItems[10];
	CheckboxTemplate CheckboxItems[25];
	SelectTemplate SelectItems[25];
	SliderTemplate SliderItems[25];
	BindTemplate BindItems[25];
	InputTemplate InputItems[25];
	ColorPickerTemplate ColorPickerItems[25];
	ListTemplate ListItem;

	__forceinline bool IsMouseHoveringRect(Vector2 Rect, Vector2 Size, Vector2 Dot)
	{
		return (Dot.x > Rect.x && Dot.x < Rect.x + Size.x && Dot.y > Rect.y && Dot.y < Rect.y + Size.y);
	}

	__forceinline const wchar_t* GetKeyName(int Key) {
		switch (Key)
		{
		case 0x01: return E(L"LButton");
		case 0x02: return E(L"RButton");
		case 0x03: return E(L"Cancel");
		case 0x04: return E(L"MButton");
		case 0x05: return E(L"XButton1");
		case 0x06: return E(L"XButton2");
		case 0x08: return E(L"Backspace");
		case 0x09: return E(L"Tab");
		case 0x0C: return E(L"Clear");
		case 0x0D: return E(L"Enter");
		case 0x10: return E(L"Shift");
		case 0x11: return E(L"Control");
		case 0x12: return E(L"Alt");
		case 0x13: return E(L"Pause");
		case 0x14: return E(L"Capslock");
		case 0x1B: return E(L"Esc");
		case 0x20: return E(L"Spacebar");
		case 0x21: return E(L"Page UP");
		case 0x22: return E(L"Page DOWN");
		case 0x23: return E(L"End");
		case 0x24: return E(L"Home");
		case 0x25: return E(L"Left Arrow");
		case 0x26: return E(L"Up Arrow");
		case 0x27: return E(L"Right Arrow");
		case 0x28: return E(L"Down Arrow");
		case 0x29: return E(L"Select");
		case 0x2A: return E(L"Print");
		case 0x2B: return E(L"Execute");
		case 0x2C: return E(L"Print Screen");
		case 0x2D: return E(L"Insert");
		case 0x2E: return E(L"Delete");
		case 0x2F: return E(L"Help");
		case 0x30: return E(L"0");
		case 0x31: return E(L"1");
		case 0x32: return E(L"2");
		case 0x33: return E(L"3");
		case 0x34: return E(L"4");
		case 0x35: return E(L"5");
		case 0x36: return E(L"6");
		case 0x37: return E(L"7");
		case 0x38: return E(L"8");
		case 0x39: return E(L"9");
		case 0x41: return E(L"A");
		case 0x42: return E(L"B");
		case 0x43: return E(L"C");
		case 0x44: return E(L"D");
		case 0x45: return E(L"E");
		case 0x46: return E(L"F");
		case 0x47: return E(L"G");
		case 0x48: return E(L"H");
		case 0x49: return E(L"I");
		case 0x4A: return E(L"J");
		case 0x4B: return E(L"K");
		case 0x4C: return E(L"L");
		case 0x4D: return E(L"M");
		case 0x4E: return E(L"N");
		case 0x4F: return E(L"O");
		case 0x50: return E(L"P");
		case 0x51: return E(L"Q");
		case 0x52: return E(L"R");
		case 0x53: return E(L"S");
		case 0x54: return E(L"T");
		case 0x55: return E(L"U");
		case 0x56: return E(L"V");
		case 0x57: return E(L"W");
		case 0x58: return E(L"X");
		case 0x59: return E(L"Y");
		case 0x5A: return E(L"Z");

		case 0x60: return E(L"NUM 0");
		case 0x61: return E(L"NUM 1");
		case 0x62: return E(L"NUM 2");
		case 0x63: return E(L"NUM 3");
		case 0x64: return E(L"NUM 4");
		case 0x65: return E(L"NUM 5");
		case 0x66: return E(L"NUM 6");
		case 0x67: return E(L"NUM 7");
		case 0x68: return E(L"NUM 8");
		case 0x69: return E(L"NUM 9");
		case 0x6A: return E(L"Multiply");
		case 0x6B: return E(L"Add");
		case 0x6C: return E(L"Seperator");
		case 0x6D: return E(L"Subtract");
		case 0x6E: return E(L"Decimal");
		case 0x6F: return E(L"Divide");
		case 0x70: return E(L"F1");
		case 0x71: return E(L"F2");
		case 0x72: return E(L"F3");
		case 0x73: return E(L"F4");
		case 0x74: return E(L"F5");
		case 0x75: return E(L"F6");
		case 0x76: return E(L"F7");
		case 0x77: return E(L"F8");
		case 0x78: return E(L"F9");
		case 0x79: return E(L"F10");
		case 0x7A: return E(L"F11");
		case 0x7B: return E(L"F12");

		case 0x90: return E(L"Numlock");
		case 0x91: return E(L"Scrollock");
		case 0xA0: return E(L"LShift");
		case 0xA1: return E(L"RShift");
		case 0xA2: return E(L"LControl");
		case 0xA3: return E(L"RControl");
		case 0xA4: return E(L"LAlt");
		case 0xA5: return E(L"RAlt");

		default: return E(L"Unknown Key");
		}
	}

	__forceinline unsigned short HashStr(const wchar_t* Str)
	{
		unsigned char i;
		unsigned short Crc = 0xFFFF;
		while (wchar_t DChar = *Str++) {
			unsigned char Char = (unsigned char)DChar;
			Crc ^= Char << 8;
			for (i = 0; i < 8; i++)
				Crc = Crc & 0x8000 ? (Crc << 1) ^ 0x1021 : Crc << 1;
			Char = (unsigned char)(DChar >> 8);
			Crc ^= Char << 8;
			for (i = 0; i < 8; i++)
				Crc = Crc & 0x8000 ? (Crc << 1) ^ 0x1021 : Crc << 1;
		} return Crc;
	}

	__forceinline bool InputMgr(const wchar_t* Name, bool Reg = false) {
		unsigned short StrHash = HashStr(Name);

		if (Reg && !IO.CurrentElement) {
			IO.CurrentElement = StrHash;
			return true;
		}
		else if (!Reg) {
			return (IO.CurrentElement == StrHash);
		}

		return false;
	}

	bool Begin(Vector2 CursorPosition, Vector2 WindowSize) {
		if (!WindowSize.x || !WindowSize.y)
			return false;

		static bool Init;
		if (!Init) {
			Window.Position.x = (WindowSize.x / 2) - 200.f;
			Init = true;
		}

		if (!Window.WaitKey && GetAsyncKeyState(0x2D) & 0x1) 
			Window.Active = !Window.Active;

		if (!Window.Active) return false;

		if (!GetAsyncKeyState(0x1)) 
			IO.CurrentElement = 0;

		Window.ParentSize = WindowSize;
		IO.MousePosition = CursorPosition;
		IO.MouseDelta = IO.MousePosition - IO.PreviousMousePosition;
		IO.PreviousMousePosition = IO.MousePosition;

		return true;
	}
	void End() {
		Window.Size.y += (Window.TabsSize.y > Window.ContentSize.y ? Window.TabsSize.y : Window.ContentSize.y);

		Texture->FillRectangle(Window.Position, { Window.Size.x, Window.Size.y }, RGB(15, 13, 18));
		Texture->Rectangle({ Window.Position.x + 12.f, Window.Position.y + 40.f }, { Window.ContentSize.x, Window.TabsSize.y > Window.ContentSize.y ? Window.TabsSize.y : Window.ContentSize.y }, RGB(36, 34, 39));
		Texture->Line({ Window.Position.x + 120.f, Window.Position.y + 41.f }, { Window.Position.x + 120.f, Window.Position.y + 39.f + (Window.TabsSize.y > Window.ContentSize.y ? Window.TabsSize.y : Window.ContentSize.y) }, RGB(36, 34, 39));
		Texture->String({ Window.Position.x + 24.f, Window.Position.y + 13.f }, E(L"//PIZZAHAX//"), RGB(255, 255, 255));

		Texture->FillRectangle({ Window.Position.x + 206.f, Window.Position.y + Window.Size.y - 13.f }, { 172.f, 1.f }, RGB(15, 13, 18));
		Texture->String({ Window.Position.x + 211.f, Window.Position.y + Window.Size.y - 20.f }, E(L"cout << \"\#FUCK_BE&EAC\"\;"), RGB(75, 71, 79));

		Texture->Bitmap((ULONG*)PizzaLogo, { Window.Position.x + 354.f, Window.Position.y + 12.f  }, { 22.f, 22.f });

		for (int i = 0; i < Items.Tab; i++) {
			bool IsHover = IsMouseHoveringRect(TabItems[i].Position, { 84.f, 15.f }, IO.MousePosition);

			if (GetAsyncKeyState(0x1) && IsHover && InputMgr(TabItems[i].Name, true))
				Window.TabIndex = i;

			ULONG Color = RGB(135, 134, 136);
			if (Window.TabIndex == i) Color = RGB(255, 255, 255);
			else if (IsHover) Color = RGB(195, 194, 196);

			Texture->String(TabItems[i].Position, TabItems[i].Name, Color);
		}
		for (int i = 0; i < Items.Checkbox; i++) {
			Texture->String({ CheckboxItems[i].Position.x + 32.f, CheckboxItems[i].Position.y + 3.f }, CheckboxItems[i].Name, RGB(255, 255, 255));
			
			if (StrLen(CheckboxItems[i].Desc)) {
				Vector2 NameSize = Texture->TextSize(CheckboxItems[i].Name);
				Texture->String({ CheckboxItems[i].Position.x + 40.f + NameSize.x , CheckboxItems[i].Position.y + 3.f }, CheckboxItems[i].Desc, CheckboxItems[i].DescColor);
			}
			
			Texture->FillRectangle(CheckboxItems[i].Position, { 20.f, 20.f }, RGB(36, 34, 39));

			if (CheckboxItems[i].State) {
				if (IsMouseHoveringRect(CheckboxItems[i].Position, { 244.f, 20.f }, IO.MousePosition))
					Texture->Bitmap((ULONG*)CheckmarkHover, { CheckboxItems[i].Position.x + 3.f , CheckboxItems[i].Position.y + 4.f }, { 14.f, 11.f });
				else
					Texture->Bitmap((ULONG*)Checkmark, { CheckboxItems[i].Position.x + 3.f , CheckboxItems[i].Position.y + 4.f }, { 14.f, 11.f });
			}
		}
		for (int i = 0; i < Items.Slider; i++) {
			bool IsHover = IsMouseHoveringRect({ SliderItems[i].Position.x + 3.f, SliderItems[i].Position.y + 23.f }, { 241.f, 24.f }, IO.MousePosition);
			Texture->String(SliderItems[i].Position, SliderItems[i].Name, RGB(255, 255, 255));

			if (StrLen(SliderItems[i].Desc)) {
				Vector2 NameSize = Texture->TextSize(SliderItems[i].Name);
				Texture->String({ SliderItems[i].Position.x + NameSize.x + 8.f, SliderItems[i].Position.y }, SliderItems[i].Desc, SliderItems[i].DescColor);
			}

			Texture->FillRectangle({ SliderItems[i].Position.x, SliderItems[i].Position.y + 23.f }, { 244.f, 24.f }, RGB(26, 24, 29));
			Texture->FillRectangle({ SliderItems[i].Position.x + 3.f + (230.f * SliderItems[i].Offset), SliderItems[i].Position.y + 25.f }, { 8.f, 20.f }, IsHover ? RGB(141, 52, 229) : RGB(118, 38, 198));
			wchar_t Str[64]; TextFormatW(Str, sizeof(Str), E(L"%d"), (int)SliderItems[i].Value);
			Texture->String({ SliderItems[i].Position.x + 111.f, SliderItems[i].Position.y + 28.f }, Str, RGB(255, 255, 255), true);
		}
		for (int i = 0; i < Items.Input; i++) {
			Texture->String({ InputItems[i].Position.x, InputItems[i].Position.y }, InputItems[i].Name, RGB(255, 255, 255));
			Texture->FillRectangle({ InputItems[i].Position.x, InputItems[i].Position.y + 23.f }, { 244.f, 24.f }, RGB(26, 24, 29));

			if (StrLen(InputItems[i].Desc)) {
				Vector2 NameSize = Texture->TextSize(InputItems[i].Name);
				Texture->String({ InputItems[i].Position.x + NameSize.x + 8.f, InputItems[i].Position.y }, InputItems[i].Desc, InputItems[i].DescColor);
			}

			int ValueLength = StrLen(InputItems[i].Value);
			Vector2 ValueSize = Texture->TextSize(InputItems[i].Value);

			if (ValueLength) {
				Texture->String({ InputItems[i].Position.x + 122.f, InputItems[i].Position.y + 28.f }, InputItems[i].Value, RGB(255, 255, 255), true);
			} else {
				if(!InputItems[i].Selected) 
					Texture->String({ InputItems[i].Position.x + 122.f, InputItems[i].Position.y + 28.f }, E(L"Enter numbers"), RGB(135, 134, 136), true);
			}
		
			static BYTE Tick = 0;
			if (InputItems[i].Selected) {
				Texture->FillRectangle({ InputItems[i].Position.x + 122.f + (ValueLength ? (ValueSize.x / 2.f + 3.f)  : 0.f), InputItems[i].Position.y + 26.f }, { 1.f, 18.f }, (Tick++ && Tick > 30 && Tick < 40) ? RGB(26, 24, 29) : RGB(135, 134, 136));
			}
		}
		for (int i = 0; i < Items.Bind; i++) {
			Texture->String({ BindItems[i].Position.x, BindItems[i].Position.y }, BindItems[i].Name, RGB(255, 255, 255));
			Texture->FillRectangle({ BindItems[i].Position.x, BindItems[i].Position.y + 23.f }, { 244.f, 24.f }, RGB(26, 24, 29));

			if (Window.WaitKey) {
				Texture->String({ BindItems[i].Position.x + 122.f, BindItems[i].Position.y + 28.f }, E(L"Press any key"), RGB(135, 134, 136), true);
			} else {
				Texture->String({ BindItems[i].Position.x + 122.f, BindItems[i].Position.y + 28.f }, BindItems[i].Key, RGB(255, 255, 255), true);
			}
		}
		for (int i = 0; i < Items.Select; i++) {
			Texture->String({ SelectItems[i].Position.x, SelectItems[i].Position.y }, SelectItems[i].Name, RGB(255, 255, 255));
			Texture->FillRectangle({ SelectItems[i].Position.x, SelectItems[i].Position.y + 23.f }, { 244.f, 24.f }, RGB(26, 24, 29));

			if (IsMouseHoveringRect({ SelectItems[i].Position.x, SelectItems[i].Position.y + 23.f }, { 50.f, 24.f }, IO.MousePosition)) 
				Texture->Bitmap((ULONG*)ArrowLeftHover, { SelectItems[i].Position.x + 12.f, SelectItems[i].Position.y + 31.f }, { 8.f, 8.f });
			else 
				Texture->Bitmap((ULONG*)ArrowLeft, { SelectItems[i].Position.x + 12.f, SelectItems[i].Position.y + 31.f }, { 8.f, 8.f });

			if (IsMouseHoveringRect({ SelectItems[i].Position.x + 194.f, SelectItems[i].Position.y + 23.f }, { 50.f, 24.f }, IO.MousePosition))
				Texture->Bitmap((ULONG*)ArrowRightHover, { SelectItems[i].Position.x + 224.f, SelectItems[i].Position.y + 31.f }, { 8.f, 8.f });
			else
				Texture->Bitmap((ULONG*)ArrowRight, { SelectItems[i].Position.x + 224.f, SelectItems[i].Position.y + 31.f }, { 8.f, 8.f });
			
			Texture->String({ SelectItems[i].Position.x + 122.f, SelectItems[i].Position.y + 28.f }, SelectItems[i].State, RGB(255, 255, 255), true);
		}
		for (int i = 0; i < Items.ColorPicker; i++) {
			Texture->String(ColorPickerItems[i].Position, ColorPickerItems[i].Name, RGB(255, 255, 255));
			Texture->FillRectangle({ ColorPickerItems[i].Position.x, ColorPickerItems[i].Position.y + 23.f }, { 20.f, 20.f }, ColorPickerItems[i].Color.Data);


			Texture->String({ ColorPickerItems[i].Position.x + 32.f, ColorPickerItems[i].Position.y + 26.f }, E(L"R"), RGB(255, 255, 255));
			Texture->FillRectangle({ ColorPickerItems[i].Position.x + 45.f, ColorPickerItems[i].Position.y + 23.f }, { 41.f, 20.f }, RGB(26, 24, 29));
			wchar_t R[6]; TextFormatW(R, sizeof(R), E(L"%d"), ColorPickerItems[i].Color.RGBA.R);
			Texture->String({ ColorPickerItems[i].Position.x + 65.f, ColorPickerItems[i].Position.y + 26.f }, R, RGB(255, 255, 255), true);

			Texture->String({ ColorPickerItems[i].Position.x + 98.f, ColorPickerItems[i].Position.y + 26.f }, E(L"G"), RGB(255, 255, 255));
			Texture->FillRectangle({ ColorPickerItems[i].Position.x + 111.f, ColorPickerItems[i].Position.y + 23.f }, { 41.f, 20.f }, RGB(26, 24, 29));
			wchar_t G[6]; TextFormatW(G, sizeof(G), E(L"%d"), ColorPickerItems[i].Color.RGBA.G);
			Texture->String({ ColorPickerItems[i].Position.x + 131.f, ColorPickerItems[i].Position.y + 26.f }, G, RGB(255, 255, 255), true);

			Texture->String({ ColorPickerItems[i].Position.x + 164.f, ColorPickerItems[i].Position.y + 26.f }, E(L"B"), RGB(255, 255, 255));
			Texture->FillRectangle({ ColorPickerItems[i].Position.x + 177.f, ColorPickerItems[i].Position.y + 23.f }, { 41.f, 20.f }, RGB(26, 24, 29));
			wchar_t B[6]; TextFormatW(B, sizeof(B), E(L"%d"), ColorPickerItems[i].Color.RGBA.B);
			Texture->String({ ColorPickerItems[i].Position.x + 197.f, ColorPickerItems[i].Position.y + 26.f }, B, RGB(255, 255, 255), true);
		}

		if (ListItem.IsVisible) {
			Texture->String({ Window.ItemPosition.x + 122.f, Window.ItemPosition.y }, ListItem.Label, RGB(255, 255, 255), true);
			Texture->FillRectangle({ Window.ItemPosition.x, Window.ItemPosition.y + 23.f }, { 208.f, 24.f }, RGB(26, 24, 29));
			
			if (IsMouseHoveringRect({ Window.ItemPosition.x + 219.f, Window.ItemPosition.y + 23.f }, { 208.f, 24.f }, IO.MousePosition)) 
				Texture->FillRectangle({ Window.ItemPosition.x + 219.f, Window.ItemPosition.y + 23.f }, { 25.f, 24.f }, RGB(141, 52, 229));
			 else
				Texture->FillRectangle({ Window.ItemPosition.x + 219.f, Window.ItemPosition.y + 23.f }, { 25.f, 24.f }, RGB(118, 38, 198) );

			Texture->Bitmap((ULONG*)Plus, { Window.ItemPosition.x + 228.f, Window.ItemPosition.y + 31.f }, { 7.f, 7.f });

			int ValueLength = StrLen(ListItem.Value);
			Vector2 ValueSize = Texture->TextSize(ListItem.Value);

			if (ValueLength)
				Texture->String({ Window.ItemPosition.x + 12.f, Window.ItemPosition.y + 28.f }, ListItem.Value, RGB(255, 255, 255));
			else
				if(!ListItem.Selected) Texture->String({ Window.ItemPosition.x + 12.f,  Window.ItemPosition.y + 28.f }, ListItem.Placeholder, RGB(135, 134, 136));

			static BYTE Tick = 0;
			if (ListItem.Selected) 
				Texture->FillRectangle({ Window.ItemPosition.x + 12.f + (ValueLength ? (ValueSize.x + 3.f) : 0.f),  Window.ItemPosition.y + 26.f }, { 1.f, 18.f }, (Tick++ && Tick > 30 && Tick < 40) ? RGB(26, 24, 29) : RGB(135, 134, 136));
			
			float LastItemPosition = Window.ItemPosition.y;
			for (int i = 0; i < ListItem.ItemsCount; i++) {
				LastItemPosition += i ? 28.f : 60.f;
				Texture->String({ Window.ItemPosition.x + 12.f, LastItemPosition }, ListItem.Items[i], RGB(255, 255, 255));
				bool IsHover = IsMouseHoveringRect({ Window.ItemPosition.x + 227.f, LastItemPosition + 3.f }, { 10.f, 10.f }, IO.MousePosition);
				Texture->Bitmap(IsHover ? (ULONG*)CloseHover : (ULONG*)Close, { Window.ItemPosition.x + 227.f, LastItemPosition + 3.f }, { 10.f, 10.f });
				if (GetAsyncKeyState(0x1) && IsHover && InputMgr(ListItem.Label, true)) ListItem.DeleteIndex = i;
			}
		}

		Texture->Bitmap((ULONG*)Cursor, IO.MousePosition, { 12.f, 16.f });

		if (InputMgr(E(L"##Drag")) || GetAsyncKeyState(0x1) && IsMouseHoveringRect(Window.Position, Window.Size, IO.MousePosition) && InputMgr(E(L"##Drag"), true))
			Window.Position += IO.MouseDelta;

		if (Window.Position.x < 0.f) Window.Position.x = 0.f;
		if (Window.Position.y < 0.f) Window.Position.y = 0.f;
		if (Window.Position.x + Window.Size.x > Window.ParentSize.x) Window.Position.x = Window.ParentSize.x - Window.Size.x;
		if (Window.Position.y + Window.Size.y > Window.ParentSize.y) Window.Position.y = Window.ParentSize.y - Window.Size.y;

		Window.Size = { 400.f, 52.f };
		Window.ContentSize = { 376.f, 14.f };
		Window.TabsSize = { 108.f, 14.f };
		
		Window.TabPosition = { Window.Position.x + 24.f,  Window.Position.y + 40.f };
		Window.ItemPosition = { Window.Position.x + 131.f,  Window.Position.y + 41.f };

		Items.Tab = 0;
		Items.Checkbox = 0;
		Items.Select = 0;
		Items.Slider = 0;
		Items.Bind = 0;
		Items.Input = 0;
		Items.ColorPicker = 0;
		ListItem.IsVisible = false;
	}

	bool Tab(const wchar_t* Name) {
		Window.TabPosition.y += 13.f;

		bool TabIsActive = Window.TabIndex == Items.Tab;

		MemCpy(TabItems[Items.Tab].Name, (PVOID)Name, ((StrLen(Name) + 1) * 2));

		TabItems[Items.Tab++].Position = Window.TabPosition;

		Window.TabPosition.y += 15.f;
		Window.TabsSize.y += 27.f;

		return TabIsActive;
	}
	bool Checkbox(const wchar_t* Name, bool& State, const wchar_t* Desc = 0, ULONG DescColor = RGB(255,255,255)) {
		Window.ItemPosition.y += 12.f;

		if (GetAsyncKeyState(0x1) && IsMouseHoveringRect(Window.ItemPosition, { 244.f, 20.f }, IO.MousePosition) && InputMgr(Name, true))
			State = !State;

		CheckboxItems[Items.Checkbox].State = State;

		MemCpy(CheckboxItems[Items.Checkbox].Name, (PVOID)Name, ((StrLen(Name) + 1) * 2));

		if (StrLen(Desc)) MemCpy(CheckboxItems[Items.Checkbox].Desc, (PVOID)Desc, ((StrLen(Desc) + 1) * 2));
		else MemZero(CheckboxItems[Items.Checkbox].Desc, sizeof(CheckboxItems[Items.Checkbox].Desc));

		CheckboxItems[Items.Checkbox].DescColor = DescColor;
	
		CheckboxItems[Items.Checkbox++].Position = Window.ItemPosition;

		Window.ItemPosition.y += 20.f;
		Window.ContentSize.y += 32.f;

		return State;
	}
	void Slider(const wchar_t* Name, float& Current, float Min, float Max, const wchar_t* Desc = 0, ULONG DescColor = RGB(255, 255, 255)) {
		Window.ItemPosition.y += 12.f;

		if (GetAsyncKeyState(0x1) && (IsMouseHoveringRect({ Window.ItemPosition.x + 3.f, Window.ItemPosition.y + 23.f }, { 241.f, 24.f }, IO.MousePosition) || InputMgr(Name))) {
			float Delta = IO.MousePosition.x - Window.ItemPosition.x;
			float Multiplier = Delta / 238.f; float Value = Multiplier * (Max - Min) + Min;
			Current = ((Value > Max) ? Max : ((Value < Min) ? Min : Value));
			InputMgr(Name, true);
		}

		SliderItems[Items.Slider].Value = Current;
		SliderItems[Items.Slider].Offset = (Current - Min) / (Max - Min);

		MemCpy(SliderItems[Items.Slider].Name, (PVOID)Name, ((StrLen(Name) + 1) * 2));

		if (StrLen(Desc)) MemCpy(SliderItems[Items.Slider].Desc, (PVOID)Desc, ((StrLen(Desc) + 1) * 2));
		else MemZero(SliderItems[Items.Slider].Desc, sizeof(SliderItems[Items.Slider].Desc));

		SliderItems[Items.Slider].DescColor = DescColor;

		SliderItems[Items.Slider++].Position = Window.ItemPosition;

		Window.ItemPosition.y += 48.f;
		Window.ContentSize.y += 60.f;
	}
	void InputNumber(const wchar_t* Name, int& Value, const wchar_t* Desc = 0, ULONG DescColor = RGB(255, 255, 255)) {
		Window.ItemPosition.y += 12.f;

		bool IsHover = IsMouseHoveringRect({ Window.ItemPosition.x, Window.ItemPosition.y + 23.f }, { 244.f, 24.f }, IO.MousePosition);

		if (GetAsyncKeyState(0x1) && !InputItems[Items.Input].Selected && IsHover && InputMgr(Name, true)) {
			InputItems[Items.Input].Selected = true;
		}
		if (GetAsyncKeyState(0x1) && !IsHover) InputItems[Items.Input].Selected = false;
		if (GetAsyncKeyState(0x1B)) InputItems[Items.Input].Selected = false; 
		if (GetAsyncKeyState(0x0D)) InputItems[Items.Input].Selected = false; 

		if (InputItems[Items.Input].Selected) {
			int TextLenght = StrLen(InputItems[Items.Input].Value);
			Vector2 TextSize = Texture->TextSize(InputItems[Items.Input].Value);

			for (int i = 48; i <= 57; i++) {
				if (TextSize.x < 184.f && GetAsyncKeyState(i) & 0x1)
					InputItems[Items.Input].Value[TextLenght] = i;
			}

			if (TextLenght && GetAsyncKeyState(0x8) & 0x1)
				InputItems[Items.Input].Value[--TextLenght] = 0;
		}

		if (!InputItems[Items.Input].Selected && Value > 0) {
			wchar_t TextFormat[64]; TextFormatW(TextFormat, sizeof(TextFormat), E(L"%d"), Value);
			MemCpy(InputItems[Items.Input].Value, (PVOID)TextFormat, ((StrLen(TextFormat) + 1) * 2));
		}

		if (!InputItems[Items.Input].Selected && !Value) {
			MemZero(InputItems[Items.Input].Value, sizeof(InputItems[Items.Input].Value));
		}

		Value = ImpCall(_wtoi, InputItems[Items.Input].Value);

		MemCpy(InputItems[Items.Input].Name, (PVOID)Name, ((StrLen(Name) + 1) * 2));

		if (StrLen(Desc)) MemCpy(InputItems[Items.Input].Desc, (PVOID)Desc, ((StrLen(Desc) + 1) * 2));
		else MemZero(InputItems[Items.Input].Desc, sizeof(InputItems[Items.Input].Desc));

		InputItems[Items.Input].DescColor = DescColor;

		InputItems[Items.Input++].Position = Window.ItemPosition;

		Window.ItemPosition.y += 48.f;
		Window.ContentSize.y += 60.f; 
	}
	void BindKey(const wchar_t* Name, int& Key) {
		Window.ItemPosition.y += 12.f;

		bool IsHover = IsMouseHoveringRect({ Window.ItemPosition.x, Window.ItemPosition.y + 23.f }, { 244.f, 24.f }, IO.MousePosition);

		if (!BindItems[Items.Bind].KeyUp && !Window.WaitKey && GetAsyncKeyState(0x1) && IsHover && InputMgr(Name, true)) {
			BindItems[Items.Bind].KeyUp = false;
			Window.WaitKey = true;
		}
		else if (Window.WaitKey && !GetAsyncKeyState(0x1)) {
			BindItems[Items.Bind].KeyUp = true;
		}

		if (Window.WaitKey && BindItems[Items.Bind].KeyUp) {
			for (int i = 1; i <= 256; i++) {
				if (BindItems[Items.Bind].KeyBind[i] && !GetAsyncKeyState(i)) {
					Window.WaitKey = false;
					BindItems[Items.Bind].KeyUp = false;
					BindItems[Items.Bind].KeyBind[i] = false;
				} else if (GetAsyncKeyState(i)) { Key = i; BindItems[Items.Bind].KeyBind[i] = true; }
			}
		}

		if (!Window.WaitKey) {
			const wchar_t* BindLabel = GetKeyName(Key);
			MemCpy(BindItems[Items.Bind].Key, (PVOID)BindLabel, ((StrLen(BindLabel) + 1) * 2));
		}

		MemCpy(BindItems[Items.Bind].Name, (PVOID)Name, ((StrLen(Name) + 1) * 2));

		BindItems[Items.Bind++].Position = Window.ItemPosition;

		Window.ItemPosition.y += 48.f;
		Window.ContentSize.y += 60.f;
	}
	void Select(const wchar_t* Name, const wchar_t* Label, int Size, int& State) {
		Window.ItemPosition.y += 12.f;

		if (GetAsyncKeyState(0x1) && IsMouseHoveringRect({ Window.ItemPosition.x, Window.ItemPosition.y + 23.f }, { 50.f, 24.f }, IO.MousePosition) && InputMgr(Name, true)) 
			State = State == 0 ? Size - 1 : State - 1;
		else if (GetAsyncKeyState(0x1) && IsMouseHoveringRect({ Window.ItemPosition.x + 194.f, Window.ItemPosition.y + 23.f }, { 50.f, 24.f }, IO.MousePosition) && InputMgr(Name, true))
			State = State >= Size - 1 ? 0 : State + 1;

		MemCpy(SelectItems[Items.Select].State, (PVOID)Label, ((StrLen(Label) + 1) * 2));
		MemCpy(SelectItems[Items.Select].Name, (PVOID)Name, ((StrLen(Name) + 1) * 2));

		SelectItems[Items.Select++].Position = Window.ItemPosition;
		
		Window.ItemPosition.y += 48.f;
		Window.ContentSize.y += 60.f;
	}
	void List(const wchar_t* Label, const wchar_t* Placeholder, PVOID Buffer, int& Count) {
		Window.ItemPosition.y += 12.f;

		bool IsHoverInput = IsMouseHoveringRect({ Window.ItemPosition.x, Window.ItemPosition.y + 23.f }, { 208.f, 24.f }, IO.MousePosition);
		bool IsHoverButton = IsMouseHoveringRect({ Window.ItemPosition.x + 219.f, Window.ItemPosition.y + 23.f }, { 25.f, 24.f }, IO.MousePosition);

		if (ListItem.Selected) {
			int TextLenght = StrLen(ListItem.Value);
			Vector2 TextSize = Texture->TextSize(ListItem.Value);

			if (TextSize.x < 184.f) {
				for (int i = 48; i <= 90; i++) {
					if (GetAsyncKeyState(i) & 0x1)
						ListItem.Value[TextLenght] = GetAsyncKeyState(0x10) ? ToUpper(i) : ToLower(i);
				}
			
				if (GetAsyncKeyState(0xBD) & 0x1) ListItem.Value[TextLenght] = 0x2D;
				if (GetAsyncKeyState(0xBE) & 0x1) ListItem.Value[TextLenght] = 0x2E;
				if (GetAsyncKeyState(0xBC) & 0x1) ListItem.Value[TextLenght] = 0x2C;
				if (GetAsyncKeyState(0xBB) & 0x1) ListItem.Value[TextLenght] = 0x2B;
				if (GetAsyncKeyState(0xBF) & 0x1) ListItem.Value[TextLenght] = 0x2F;
				if (GetAsyncKeyState(0xDB) & 0x1) ListItem.Value[TextLenght] = 0x28;
				if (GetAsyncKeyState(0xDD) & 0x1) ListItem.Value[TextLenght] = 0x29;
				if (GetAsyncKeyState(0xDE) & 0x1) ListItem.Value[TextLenght] = 0x22;
				if (GetAsyncKeyState(0x20) & 0x1) ListItem.Value[TextLenght] = 0x20;
			}

			if (TextLenght && GetAsyncKeyState(0x8) & 0x1)
				ListItem.Value[--TextLenght] = 0;
		}
		

		if (GetAsyncKeyState(0x1) && IsHoverInput && InputMgr(Label, true)) ListItem.Selected = true;
		if (GetAsyncKeyState(0x1) && !IsHoverInput) ListItem.Selected = false;
		if (GetAsyncKeyState(0x1B)) ListItem.Selected = false;


		if ((StrLen(ListItem.Value) && ListItem.ItemsCount < 30) && ((ListItem.Selected && GetAsyncKeyState(0x0D) & 0x1) || (IsHoverButton && GetAsyncKeyState(0x1) && InputMgr(Label, true)))) {
			MemCpy(ListItem.Items[ListItem.ItemsCount++], (PVOID)ListItem.Value, ((StrLen(ListItem.Value) + 1) * 2));
			MemZero(ListItem.Value, sizeof(ListItem.Value));
			ListItem.Selected = true;
		}

		if (ListItem.DeleteIndex != -1) {
			for (int i = ListItem.DeleteIndex; i < ListItem.ItemsCount - 1; i++) {
				MemCpy(ListItem.Items[i], (PVOID)ListItem.Items[i + 1], ((StrLen(ListItem.Items[i + 1]) + 1) * 2));
				MemZero(ListItem.Items[i + 1], sizeof(ListItem.Items[i + 1]));
			}
			ListItem.ItemsCount--;
		}

		ListItem.DeleteIndex = -1;


		ListItem.IsVisible = true;
		MemCpy(ListItem.Label, (PVOID)Label, ((StrLen(Label) + 1) * 2));
		MemCpy(ListItem.Placeholder, (PVOID)Placeholder, ((StrLen(Placeholder) + 1) * 2));

		Window.ContentSize.y += 60.f;
		Window.ContentSize.y += 28.f * ListItem.ItemsCount;
	}
	void ColorPicker(const wchar_t* Name, ULONG& Color) {
		Window.ItemPosition.y += 12.f;

		MemCpy(ColorPickerItems[Items.ColorPicker].Name, (PVOID)Name, ((StrLen(Name) + 1) * 2));

		ColorPickerItems[Items.ColorPicker].Color.Data = Color;
		ColorPickerItems[Items.ColorPicker].Color.RGBA.A = 255;

		wchar_t InputName[64]; 

		TextFormatW(InputName, sizeof(InputName), E(L"##R%ls"), Name);
		if ((GetAsyncKeyState(0x1) && (IsMouseHoveringRect({ Window.ItemPosition.x + 45.f, Window.ItemPosition.y + 23.f }, { 41.f, 20.f }, IO.MousePosition) && !ColorPickerItems[Items.ColorPicker].Picked) || InputMgr(InputName))) {
			float Delta = IO.MousePosition.x - Window.ItemPosition.x - 45.f;
			float Multiplier = Delta / 41.f; float Value = Multiplier * (255 - 1) + 1;
			ColorPickerItems[Items.ColorPicker].Color.RGBA.R = (BYTE)((Value > 255) ? 255 : ((Value < 1) ? 1 : Value));
			ColorPickerItems[Items.ColorPicker].Picked = true; InputMgr(InputName, true);
		}
		
		TextFormatW(InputName, sizeof(InputName), E(L"##G%ls"), Name);
		if ((GetAsyncKeyState(0x1) && (IsMouseHoveringRect({ Window.ItemPosition.x + 111.f, Window.ItemPosition.y + 26.f }, { 41.f, 20.f }, IO.MousePosition) && !ColorPickerItems[Items.ColorPicker].Picked) || InputMgr(InputName))) {
			float Delta = IO.MousePosition.x - Window.ItemPosition.x - 111.f;
			float Multiplier = Delta / 41.f; float Value = Multiplier * (255 - 1) + 1;
			ColorPickerItems[Items.ColorPicker].Color.RGBA.G = (BYTE)((Value > 255) ? 255 : ((Value < 1) ? 1 : Value));
			ColorPickerItems[Items.ColorPicker].Picked = true; InputMgr(InputName, true);
		}

		TextFormatW(InputName, sizeof(InputName), E(L"##B%ls"), Name);
		if ((GetAsyncKeyState(0x1) && (IsMouseHoveringRect({ Window.ItemPosition.x + 177.f,  Window.ItemPosition.y + 26.f }, { 41.f, 20.f }, IO.MousePosition) && !ColorPickerItems[Items.ColorPicker].Picked) || InputMgr(InputName))) {
			float Delta = IO.MousePosition.x - Window.ItemPosition.x - 177.f;
			float Multiplier = Delta / 41.f; float Value = Multiplier * (255 - 1) + 1;
			ColorPickerItems[Items.ColorPicker].Color.RGBA.B = (BYTE)((Value > 255) ? 255 : ((Value < 1) ? 1 : Value));
			ColorPickerItems[Items.ColorPicker].Picked = true; InputMgr(InputName, true);
		}

		if (ColorPickerItems[Items.ColorPicker].Picked && !GetAsyncKeyState(0x1)) ColorPickerItems[Items.ColorPicker].Picked = false;

		Color = ColorPickerItems[Items.ColorPicker].Color.Data;

		ColorPickerItems[Items.ColorPicker++].Position = Window.ItemPosition;

		Window.ItemPosition.y += 44.f;
		Window.ContentSize.y += 56.f;
	}
}