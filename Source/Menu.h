#pragma once
#include <array>
#include <JuceHeader.h>
#include "modsys/ModSysGUI.h"

namespace menu2
{
	using namespace gui;
	using NotificationType = gui::NotificationType;
	using Shared = gui::Shared;

	using Path = juce::Path;
	using Stroke = juce::PathStrokeType;
	using Bounds = juce::Rectangle<int>;
	using BoundsF = juce::Rectangle<float>;
	using Point = juce::Point<int>;
	using PointF = juce::Point<float>;
	using Line = juce::Line<int>;
	using LineF = juce::Line<float>;
	using Just = juce::Justification;
	using String = juce::String;
	using Graphics = juce::Graphics;
	using Colour = juce::Colour;
	using Font = juce::Font;
	using Component = juce::Component;
	using Timer = juce::Timer;
	using Mouse = juce::MouseEvent;
	using MouseWheel = juce::MouseWheelDetails;
	using Image = juce::Image;
	using Just = juce::Justification;
	using Random = juce::Random;
	using ValueTree = juce::ValueTree;
	using Identifier = juce::Identifier;

	/*
	struct ButtonM :
		public Comp
	{
		ButtonM(Utils& u, const juce::String& tooltp, std::function<void()> _onClick, std::function<void(juce::Graphics&, ButtonM&)> _onPaint) :
			Comp(u, tooltp, CursorType::Interact),
			onClick(_onClick),
			onPaint(_onPaint),
			blinkyBoy(this)
		{
		}
		BlinkyBoy& getBlinky() noexcept { return blinkyBoy; }
	protected:
		std::function<void()> onClick;
		std::function<void(juce::Graphics&, ButtonM&)> onPaint;
		BlinkyBoy blinkyBoy;

		void mouseEnter(const juce::MouseEvent& evt) override
		{
			Comp::mouseEnter(evt);
			repaint();
		}
		void mouseExit(const juce::MouseEvent&) override { repaint(); }
		void mouseUp(const juce::MouseEvent& evt) override
		{
			if (evt.mouseWasDraggedSinceMouseDown()) return;
			onClick();
			blinkyBoy.trigger(3.f);
		}
		void paint(juce::Graphics& g) override { onPaint(g, *this); }
	};
	*/

	inline void paintMenuButton(Graphics& g, Button& b, const Utils& utils, bool selected = false)
	{
		//auto& blinky = b.blinky;
		//blinky.flash(g, juce::Colours::white);
		const auto thicc = utils.thicc;
		const auto bounds = b.getLocalBounds().toFloat().reduced(thicc);
		const auto bounds2 = bounds.reduced(thicc);
		g.setColour(Shared::shared.colour(ColourID::Bg));
		g.fillRoundedRectangle(bounds2, thicc);
		juce::Colour mainCol;
		if (b.isMouseOver())
		{
			g.setColour(Shared::shared.colour(ColourID::Hover));
			g.fillRoundedRectangle(bounds2, thicc);
		}
		if (selected)
			mainCol = Shared::shared.colour(ColourID::Interact);
		else
			mainCol = Shared::shared.colour(ColourID::Txt);
		g.setFont(Shared::shared.font);
		g.setColour(mainCol);
		//blinky.flash(g, mainCol);
		g.drawRoundedRectangle(bounds, thicc, thicc);
		g.drawFittedText(b.getName(), bounds.toNearestInt(), Just::centred, 1);
	}

	/*
	struct LabelM :
		public modSys6::gui::Label
	{
		LabelM(Utils& _utils, const juce::String& _name) :
			modSys6::gui::Label(_utils, "< " + _name + " >")
		{
		}
	protected:
		void paint(Graphics& g) override
		{
			g.setFont(Shared::shared.font);
			const auto thicc = utils.thicc;
			const auto bounds = getLocalBounds().toFloat().reduced(Shared::shared.thicc);
			g.setColour(Shared::shared.colour(bgC));
			g.fillRoundedRectangle(bounds, thicc);
			g.setColour(Shared::shared.colour(outlineC));
			g.drawRoundedRectangle(bounds, thicc, thicc);
			g.setColour(Shared::shared.colour(txtC));
			g.drawFittedText(
				txt, getLocalBounds(), juce::Justification::centred, 1
			);
		}
	};
	*/
	

	struct ColourSelector :
		public Comp
	{
		ColourSelector(Utils& u, int idx, std::function<void()> _onExit) :
			Comp(u, "", CursorType::Interact),
			selector(27, 4, 7),
			okButton(u, "you accept the changes you made."),
			undoButton(u, "you want to go back to better times."),
			defaultButton(u, "back to default colour."),
			startColour(Shared::shared.colour(static_cast<ColourID>(idx))),
			colIdx(idx),
			onExit(_onExit)
		{
			okButton.onClick = [this]() { onClickOK(); };
			undoButton.onClick = [this]() { onClickUNDO(); };
			defaultButton.onClick = [this]() { onClickDEFAULT(); };

			okButton.onPaint = [this](juce::Graphics& g, Button& b) { paintMenuButton(g, b, utils); };
			undoButton.onPaint = [this](juce::Graphics& g, Button& b) { paintMenuButton(g, b, utils); };
			defaultButton.onPaint = [this](juce::Graphics& g, Button& b) { paintMenuButton(g, b, utils); };

			addAndMakeVisible(selector);
			addAndMakeVisible(okButton);		okButton.setName("OK");
			addAndMakeVisible(undoButton);		undoButton.setName("undo");
			addAndMakeVisible(defaultButton);	defaultButton.setName("default");

			selector.setCurrentColour(startColour);
		}
		void update(int cIdx, Colour col)
		{
			Shared::shared.setColour(cIdx, col);
			notify(NotificationType::ColourChanged, &colIdx);
		}
		
		void update()
		{
			update(colIdx, selector.getCurrentColour());
		}
		
	protected:
		juce::ColourSelector selector;
		Button okButton, undoButton, defaultButton;
		Colour startColour;
		const int colIdx;
		std::function<void()> onExit;

		void resized() override
		{
			const auto bounds = getLocalBounds().toFloat();
			const auto selectorBounds = maxQuadIn(bounds);
			selector.setBounds(selectorBounds.toNearestInt());
			const auto buttonsX = selectorBounds.getRight();
			auto buttonY = 0.f;
			const auto buttonsWidth = bounds.getWidth() - buttonsX;
			const auto buttonsHeight = bounds.getHeight() / 3.f;

			okButton.setBounds(BoundsF(buttonsX, buttonY, buttonsWidth, buttonsHeight).toNearestInt());
			buttonY += buttonsHeight;
			undoButton.setBounds(BoundsF(buttonsX, buttonY, buttonsWidth, buttonsHeight).toNearestInt());
			buttonY += buttonsHeight;
			defaultButton.setBounds(BoundsF(buttonsX, buttonY, buttonsWidth, buttonsHeight).toNearestInt());
		}
		
	private:
		void onClickOK()
		{
			update(colIdx, selector.getCurrentColour());
			onExit();
		}
		void onClickUNDO()
		{
			update(colIdx, startColour);
			onExit();
		}
		void onClickDEFAULT()
		{
			update(colIdx, getDefault(static_cast<ColourID>(colIdx)));
			onExit();
		}
	};

	struct SwitchButton :
		public Comp
	{
		SwitchButton(Utils& u, const juce::String& tooltp, const juce::String& name,
			std::function<void(int)> onSwitch, const std::vector<juce::var>& options,
			std::function<int(int)> onIsEnabled
		) :
			Comp(u, tooltp),
			label(u, name),
			buttons()
		{
			for (auto i = 0; i < options.size(); ++i) {
				auto onClick = [os = onSwitch, &btns = buttons, j = i]()
				{
					os(j);
					for (auto& btn : btns)
						btn->repaint();
				};
				auto onPaint = [this, isEnabled = onIsEnabled, j = i](juce::Graphics& g, Button& b)
				{
					paintMenuButton(g, b, utils, isEnabled(j));
				};

				buttons.push_back(std::make_unique<Button>
				(
					u, tooltp
				));
				auto& back = *buttons.back();
				back.onClick = onClick;
				back.onPaint = onPaint;
				buttons[i]->setName(options[i].toString());
				addAndMakeVisible(buttons[i].get());
			}
			addAndMakeVisible(label);
		}
	protected:
		Label label;
		std::vector<std::unique_ptr<Button>> buttons;

		void paint(juce::Graphics&) override {}
		
		void resized() override
		{
			auto x = 0.f;
			const auto y = 0.f;
			const auto width = static_cast<float>(getWidth());
			const auto height = static_cast<float>(getHeight());
			const auto compWidth = width / static_cast<float>(buttons.size() + 1);

			label.setBounds(BoundsF(x, y, compWidth, height).toNearestInt());
			x += compWidth;
			for (auto& btn : buttons)
			{
				btn->setBounds(BoundsF(x, y, compWidth, height).toNearestInt());
				x += compWidth;
			}
		}
	};

	struct TextBox :
		public Comp,
		public juce::Timer
	{
		TextBox(Utils& u, const juce::String& tooltp, const juce::String& _name,
			std::function<bool(const juce::String& txt)> _onUpdate, std::function<juce::String()> onDefaultText, juce::String&& _unit = "") :
			Comp(u, tooltp),
			blinkyBoy(this),
			onUpdate(_onUpdate),
			txt(onDefaultText()),
			txtDefault(txt),
			unit(_unit),
			pos(txt.length()),
			showTick(false)
		{
			setName(_name);
			setWantsKeyboardFocus(true);
		}
	protected:
		BlinkyBoy blinkyBoy;
		std::function<bool(const juce::String& txt)> onUpdate;
		juce::String txt, txtDefault, unit;
		int pos;
		bool showTick;

		void mouseUp(const Mouse& evt) override
		{
			if (evt.mouseWasDraggedSinceMouseDown()) return;
			startTimer(static_cast<int>(1000.f / 3.f));
			grabKeyboardFocus();
		}

		void paint(Graphics& g) override
		{
			blinkyBoy.flash(g, juce::Colours::white);

			const auto thicc = utils.thicc;
			const auto bounds = getLocalBounds().toFloat().reduced(thicc);
			g.setFont(Shared::shared.font);
			g.setColour(Shared::shared.colour(ColourID::Bg));
			g.fillRoundedRectangle(bounds, thicc);
			blinkyBoy.flash(g, Shared::shared.colour(ColourID::Interact));
			g.setColour(Shared::shared.colour(ColourID::Interact));
			g.drawRoundedRectangle(bounds, thicc, thicc);
			if (showTick)
			{
				const auto txt2Paint = getName() + ": " + txt.substring(0, pos) + "|" + txt.substring(pos) + unit;
				g.drawFittedText(txt2Paint, getLocalBounds(), juce::Justification::centred, 1);
			}
			else
				g.drawFittedText(getName() + ": " + txt + unit, getLocalBounds(), juce::Justification::centred, 1);
		}
		void timerCallback() override
		{
			if (!hasKeyboardFocus(true)) return;
			showTick = !showTick;
			repaint();
		}

		bool keyPressed(const juce::KeyPress& key) override
		{
			const auto code = key.getKeyCode();
			if (code == juce::KeyPress::escapeKey)
			{
				backToDefault();
				repaintWithTick();
			}
			else if (code == juce::KeyPress::backspaceKey)
			{
				if (txt.length() > 0)
				{
					txt = txt.substring(0, pos - 1) + txt.substring(pos);
					--pos;
					repaintWithTick();
				}
			}
			else if (code == juce::KeyPress::deleteKey)
			{
				if (txt.length() > 0)
				{
					txt = txt.substring(0, pos) + txt.substring(pos + 1);
					repaintWithTick();
				}
			}
			else if (code == juce::KeyPress::leftKey)
			{
				if (pos > 0)
					--pos;
				repaintWithTick();
			}
			else if (code == juce::KeyPress::rightKey)
			{
				if (pos < txt.length())
					++pos;
				repaintWithTick();
			}
			else if (code == juce::KeyPress::returnKey)
			{
				bool successful = onUpdate(txt);
				if (successful)
				{
					blinkyBoy.trigger(3.f);
					txtDefault = txt;
					stopTimer();
					giveAwayKeyboardFocus();
				}
				else
					backToDefault();
				repaintWithTick();
			}
			else
			{
				// character is text
				txt = txt.substring(0, pos) + key.getTextCharacter() + txt.substring(pos);
				++pos;
				repaintWithTick();
			}
			return true;
		}
		void repaintWithTick()
		{
			showTick = true;
			repaint();
		}
		void backToDefault()
		{
			txt = txtDefault;
			pos = txt.length();
		}
	};

	struct ImageStrip :
		public Comp
	{
		using Img = std::unique_ptr<ImageComp>;
		using Images = std::vector<Img>;

		ImageStrip(Utils& u) :
			Comp(u, "", CursorType::Default),
			images()
		{
			setBufferedToImage(false);
			setInterceptsMouseClicks(false, true);
		}
		
		void addImage(const char* data, const int size, String&& _tooltip)
		{
			images.push_back(std::make_unique<ImageComp>
			(
				utils, std::move(_tooltip), data, size
			));
			addAndMakeVisible(images.back().get());
		}
		
	protected:
		Images images;

		void paint(juce::Graphics&) override {}

		void resized() override
		{
			const auto thicc = utils.thicc;
			const auto bounds = getLocalBounds().toFloat().reduced(thicc);
			auto x = bounds.getX();
			const auto y = bounds.getY();
			const auto w = bounds.getWidth() / static_cast<float>(images.size());
			const auto h = bounds.getHeight();
			for (auto& img : images)
			{
				img->setBounds(BoundsF(x,y,w,h).reduced(thicc).toNearestInt());
				x += w;
			}
		}
	};

	struct TextComp :
		public Comp
	{
		TextComp(Utils& u, String&& _txt) :
			Comp(u, "", CursorType::Default),
			txt(_txt)
		{

		}
	protected:
		String txt;

		void paint(Graphics& g)
		{
			const auto thicc = utils.thicc;
			const auto bounds = getLocalBounds().toFloat().reduced(thicc);
			g.setColour(Shared::shared.colour(ColourID::Txt));
			g.drawFittedText(txt, bounds.toNearestInt(), Just::left, 100);
		}
	};

	struct Link :
		public Comp
	{
		Link(Utils& u, String&& _tooltip, String&& _name, String&& _link) :
			Comp(u, ""),
			button(u, std::move(_tooltip))
		{
			setBufferedToImage(false);
			setInterceptsMouseClicks(false, true);
			button.onPaint = makeTextButtonOnPaint(std::move(_name));
			button.onClick = [&b = button, link = _link]()
			{
				juce::URL url(link);
				url.launchInDefaultBrowser();
				b.repaint();
			};
			addAndMakeVisible(button);
		}
	protected:
		gui::Button button;

		void paint(Graphics&) override {}
		
		void resized() override
		{
			button.setBounds(getLocalBounds());
		}
	};

	struct LinkStrip :
		public Comp
	{
		using HyperLink = std::unique_ptr<Link>;

		LinkStrip(Utils& u) :
			Comp(u, "")
		{
			setBufferedToImage(false);
			setInterceptsMouseClicks(false, true);
		}
		
		void addLink(juce::String&& _tooltip, juce::String&& _name, juce::String&& _link)
		{
			links.push_back(std::make_unique<Link>
			(
				utils, std::move(_tooltip), std::move(_name), std::move(_link)
			));
			addAndMakeVisible(links.back().get());
		}
	protected:
		std::vector<HyperLink> links;

		void paint(Graphics&) override
		{}
		
		void resized() override
		{
			const auto thicc = utils.thicc;
			const auto bounds = getLocalBounds().toFloat().reduced(thicc);
			auto x = bounds.getX();
			const auto y = bounds.getY();
			const auto w = bounds.getWidth() / static_cast<float>(links.size());
			const auto h = bounds.getHeight();
			for (auto& link : links)
			{
				link->setBounds(BoundsF(x, y, w, h).reduced(thicc).toNearestInt());
				x += w;
			}
		}
	};

	class Menu :
		public Comp,
		public juce::Timer
	{
		enum ID { ID, MENU, TOOLTIP, COLOURSELECTOR, SWITCH, TEXTBOX, IMGSTRIP, TXT, LINK, LINKSTRIP, NumIDs };
	public:
		Menu(Nel19AudioProcessor& p, Utils& u, juce::ValueTree _xml, Menu* _parent = nullptr) :
			Comp(u, ""),
			juce::Timer(),
			processor(p),
			xml(_xml),
			nameLabel(u, xml.getProperty("id", "")),
			subMenu(nullptr),
			colourSelector(nullptr),
			parent(_parent)
		{
			std::array<juce::Identifier, NumIDs> id = { "id", "menu", "tooltip", "colourselector", "switch", "textbox", "imgStrip", "txt", "link", "linkstrip" };
			addEntries(id);
			addAndMakeVisible(nameLabel);
			startTimerHz(12);
		}
	protected:
		Nel19AudioProcessor& processor;
		ValueTree xml;
		Label nameLabel;
		std::vector<std::unique_ptr<Comp>> entries;
		std::unique_ptr<Menu> subMenu;
		std::unique_ptr<ColourSelector> colourSelector;
		Menu* parent;

		void setVisibleAllMenus(bool e)
		{
			Comp::setVisible(e);
			if (parent != nullptr)
				parent->setVisibleAllMenus(e);
		}

		void paint(juce::Graphics& g) override
		{
			g.fillAll(Shared::shared.colour(ColourID::Bg));
		}
		
		void resized() override
		{
			const auto width = static_cast<float>(getWidth());
			const auto height = static_cast<float>(getHeight());
			const auto thicc = utils.thicc;
			const auto thicc2 = thicc * 2.f;
			BoundsF nameLabelBounds;
			{
				const auto& font = Shared::shared.font;
				const auto w = static_cast<float>(font.getStringWidth(nameLabel.getText())) + thicc2;
				const auto h = height * .2f;
				const auto x = (width - w) * .5f;
				const auto y = 0.f;
				nameLabelBounds.setBounds(x, y, w, h);
				nameLabel.setBounds(nameLabelBounds.toNearestInt());
			}
			if (entries.size() == 0) return;
			const auto entriesY = nameLabelBounds.getBottom();
			const auto entriesHeight = height - entriesY;
			BoundsF entriesBounds(0.f, entriesY, width, entriesHeight);
			const auto entryHeight = entriesHeight / static_cast<float>(entries.size());
			auto entryY = entriesY;
			for (auto& e : entries)
			{
				e->setBounds(BoundsF(0.f, entryY, width, entryHeight).reduced(1).toNearestInt());
				entryY += entryHeight;
			}
		}
		
		void timerCallback() override
		{
			if (colourSelector != nullptr && colourSelector->isVisible())
				colourSelector->update();
		}
	private:
		
		void addEntries(const std::array<juce::Identifier, NumIDs>& id)
		{
			for (auto i = 0; i < xml.getNumChildren(); ++i)
			{
				const auto child = xml.getChild(i);
				const auto type = child.getType();
				if (type == id[MENU])
					addSubMenuButton(id, child, i);
				else if (type == id[COLOURSELECTOR])
					addColourSelector(id, child, i);
				else if (type == id[SWITCH])
					addSwitchButton(id, child, i);
				else if (type == id[TEXTBOX])
					addTextBox(id, child, i);
				else if (type == id[IMGSTRIP])
					addImgStrip(id, child, i);
				else if (type == id[TXT])
					addText(id, child, i);
				else if (type == id[LINK])
					addLink(id, child, i);
				else if (type == id[LINKSTRIP])
					addLinkStrip(id, child, i);
			}
		}

		void addSubMenuButton(const std::array<juce::Identifier, NumIDs>& id, juce::ValueTree child, const int i)
		{
			const auto onClick = [this, idx = i]()
			{
				if (subMenu == nullptr)
				{
					auto nXml = xml.getChild(idx);
					subMenu.reset(new Menu(processor, utils, nXml, this));
					addAndMakeVisible(*subMenu.get());
					subMenu->setBounds(getLocalBounds());
				}
			};
			
			const auto onPaint = [this](juce::Graphics& g, Button& b)
			{
				paintMenuButton(g, b, utils);
			};
			
			const auto tooltp = child.getProperty(id[TOOLTIP]);
			entries.push_back(std::make_unique<Button>(utils, tooltp.toString()));
			auto& btn = *reinterpret_cast<Button*>(entries.back().get());

			btn.onClick = onClick;
			btn.onPaint = onPaint;
			const auto buttonName = child.getProperty(id[ID]);
			entries.back()->setName(buttonName.toString());
			addAndMakeVisible(*entries.back().get());
		}
		
		void addColourSelector(const std::array<juce::Identifier, NumIDs>& id, juce::ValueTree child, const int i)
		{
			const auto onClick = [this, idx = i]()
			{
				const auto onExit = [this]()
				{
					auto top = getTopLevelComponent();
					colourSelector->setVisible(false);
					setVisibleAllMenus(true);
					repaintWithChildren(top);
				};
				colourSelector.reset(new ColourSelector(utils, idx, onExit));
				auto top = getTopLevelComponent();
				top->addAndMakeVisible(*colourSelector.get());
				const auto bounds = top->getBounds();
				const auto minDimen = std::min(bounds.getWidth(), bounds.getHeight());
				const auto reduced = minDimen / 6;
				colourSelector->setBounds(top->getLocalBounds().reduced(reduced));
				setVisibleAllMenus(false);
			};
			const auto onPaint = [this](Graphics& g, Button& b)
			{
				paintMenuButton(g, b, utils);
			};
			const auto buttonName = child.getProperty(id[ID]).toString();
			const auto tooltp = String("adjust the colour of ") + buttonName + String(" UI elements.");
			entries.push_back(std::make_unique<Button>(utils, tooltp));
			auto& btn = *reinterpret_cast<Button*>(entries.back().get());
			btn.onClick = onClick;
			btn.onPaint = onPaint;
			btn.setName(buttonName);
			addAndMakeVisible(btn);
		}
		
		void addSwitchButton(const std::array<juce::Identifier, NumIDs>& id, juce::ValueTree child, const int,
			std::function<void(int)> onSwitch, const juce::String& buttonName, std::function<bool(int)> onIsEnabled)
		{
			const auto tooltp = child.getProperty(id[TOOLTIP]);
			std::vector<juce::var> options;
			for (auto c = 0; c < child.getNumChildren(); ++c)
			{
				auto optionChild = child.getChild(c);
				options.push_back(optionChild.getProperty(id[ID]));
			}
			entries.push_back(std::make_unique<SwitchButton>
			(
				utils, tooltp.toString(), buttonName, onSwitch, options, onIsEnabled
			));
			addAndMakeVisible(*entries.back().get());
		}
		
		void addSwitchButton(const std::array<juce::Identifier, NumIDs>& id,
			juce::ValueTree child, const int i)
		{
			const auto buttonName = child.getProperty(id[ID]).toString();
			// buttonName must match id in menu.xml
			if (buttonName == "tooltips")
			{
				const auto onSwitch = [this](int e)
				{
					bool eb = e == 0 ? false : true;
					utils.setTooltipsEnabled(eb);
				};
				const auto onIsEnabled = [this](int i)
				{
					return (utils.getTooltipsEnabled() ? 1 : 0) == i;
				};
				addSwitchButton(id, child, i, onSwitch, buttonName, onIsEnabled);
			}
			else if (buttonName.contains("modType"))
			{
				auto mIdx = 0;
				for (auto j = 0; j < buttonName.length(); ++j)
					if (buttonName[j] == '1')
					{
						mIdx = 1;
						break;
					}
				const auto onSwitch = [this, mIdx](int e)
				{
					const auto objType = vibrato::ObjType::ModType;
					const auto objStr = vibrato::with(objType, mIdx);
					const juce::Identifier id(objStr);
					const auto modType = vibrato::ModType(e);
					const auto mID = vibrato::toString(modType);
					auto user = processor.appProperties.getUserSettings();
					user->setValue(id, mID);
				};
				const auto onIsEnabled = [this, mIdx](int i)
				{
					const auto user = processor.appProperties.getUserSettings();
					const auto objType = vibrato::ObjType::ModType;
					const auto objStr = vibrato::with(objType, mIdx);
					const juce::Identifier id(objStr);
					const auto mID = user->getValue(id);
					const auto modType = vibrato::getModType(mID);
					return static_cast<int>(modType) == i;
				};
				addSwitchButton(id, child, i, onSwitch, buttonName, onIsEnabled);
			}
		}
		
		void addTextBox(const std::array<Identifier, NumIDs>&, ValueTree, const int)
		{
			//const auto buttonName = child.getProperty(id[ID]).toString();
		}
		
		void addImgStrip(const std::array<juce::Identifier, NumIDs>& id, juce::ValueTree child, const int)
		{
			const auto stripName = child.getProperty(id[ID]).toString();
			if (stripName == "logos")
			{
				entries.push_back(std::make_unique<ImageStrip>(this->utils));

				auto strip = static_cast<ImageStrip*>(entries.back().get());
				strip->addImage
				(
					BinaryData::vst3_logo_small_png, BinaryData::vst3_logo_small_pngSize, "thanks to steinberg for having invented something that transcendents into magic occasionally."
				);
				strip->addImage
				(
					BinaryData::shuttle_png, BinaryData::shuttle_pngSize, "thanks to my son Lionel, who inspires me to push myself everyday."
				);
				strip->addImage
				(
					BinaryData::juce_png, BinaryData::juce_pngSize, "thanks to the juce framework for enabling programming noobs like me to fulfill their dreams."
				);

				addAndMakeVisible(entries.back().get());
			}
		}
		
		void addText(const std::array<juce::Identifier, NumIDs>&, juce::ValueTree child, const int)
		{
			entries.push_back(std::make_unique<TextComp>
			(
				utils, child.getProperty("text").toString()
			));
			addAndMakeVisible(entries.back().get());
		}
		
		void addLink(const std::array<juce::Identifier, NumIDs>&, juce::ValueTree child, const int)
		{
			entries.push_back(std::make_unique<Link>
			(
				utils,
				child.getProperty("tooltip").toString(),
				child.getProperty("id").toString(),
				child.getProperty("link").toString()
			));
			addAndMakeVisible(entries.back().get());
		}
		
		void addLinkStrip(const std::array<juce::Identifier, NumIDs>&,
			juce::ValueTree child, const int)
		{
			entries.push_back(std::make_unique<LinkStrip>(this->utils));

			auto strip = static_cast<LinkStrip*>(entries.back().get());

			for (auto j = 0; j < child.getNumChildren(); ++j)
			{
				const auto c = child.getChild(j);
				strip->addLink(
					c.getProperty("tooltip").toString(),
					c.getProperty("id").toString(),
					c.getProperty("link").toString()
				);
			}

			addAndMakeVisible(entries.back().get());
		}
	};

	inline void paintMenuButton(Graphics& g, Button& button, Utils& utils, Menu* menu)
	{
		const auto width = static_cast<float>(button.getWidth());
		const auto height = static_cast<float>(button.getHeight());
		const PointF centre(width, height);
		auto minDimen = std::min(width, height);
		BoundsF bounds
		(
			(width - minDimen) * .5f,
			(height - minDimen) * .5f,
			minDimen,
			minDimen
		);
		const auto thicc = utils.thicc;
		bounds.reduce(thicc, thicc);
		g.setColour(Shared::shared.colour(ColourID::Bg));
		g.fillRoundedRectangle(bounds, thicc);
		if (button.isMouseOver())
		{
			g.setColour(Shared::shared.colour(ColourID::Hover));
			g.fillRoundedRectangle(bounds, thicc);
			g.setColour(Shared::shared.colour(ColourID::Interact));
			if (menu != nullptr)
			{
				g.setColour(Shared::shared.colour(ColourID::Abort));
				g.drawRoundedRectangle(bounds, thicc, thicc);
				g.drawFittedText("X", bounds.toNearestInt(), Just::centred, 1, 0);
				return;
			}
		}
		else
			if (menu == nullptr)
				g.setColour(Shared::shared.colour(ColourID::Txt));
			else
			{
				g.setColour(Shared::shared.colour(ColourID::Abort));
				g.drawRoundedRectangle(bounds, thicc, thicc);
				g.drawFittedText("X", bounds.toNearestInt(), Just::centred, 1, 0);
				return;
			}
		g.drawRoundedRectangle(bounds, thicc, thicc);
		const auto boundsHalf = bounds.reduced(std::min(bounds.getWidth(), bounds.getHeight()) * .25f);
		g.drawEllipse(boundsHalf.reduced(thicc * .5f), thicc);
		minDimen = std::min(boundsHalf.getWidth(), boundsHalf.getHeight());
		const auto radius = minDimen * .5f;
		auto bumpSize = radius * .4f;
		LineF bump(0.f, radius, 0.f, radius + bumpSize);
		const auto translation = juce::AffineTransform::translation(bounds.getCentre());
		for (auto i = 0; i < 4; ++i)
		{
			const auto x = static_cast<float>(i) / 4.f;
			auto rotatedBump = bump;
			const auto rotation = juce::AffineTransform::rotation(x * Tau);
			rotatedBump.applyTransform(rotation.followedBy(translation));
			g.drawLine(rotatedBump, thicc);
		}
		bumpSize *= .6f;
		bump.setStart(0, radius); bump.setEnd(0, radius + bumpSize);
		for (auto i = 0; i < 4; ++i)
		{
			const auto x = static_cast<float>(i) / 4.f;
			auto rotatedBump = bump;
			const auto rotation = juce::AffineTransform::rotation(PiQuart + x * Tau);
			rotatedBump.applyTransform(rotation.followedBy(translation));
			g.drawLine(rotatedBump, thicc);
		}
	}
	
	inline std::unique_ptr<juce::XmlElement> loadXML(const char* data, const int sizeInBytes)
	{
		return juce::XmlDocument::parse(juce::String(data, sizeInBytes));
	}
	
	inline void openMenu(std::unique_ptr<Menu>& menu, Nel19AudioProcessor& p,
		Utils& utils, Component& parentComp, Bounds menuBounds,
		Button& openButton)
	{
		if (menu == nullptr)
		{
			auto xml = loadXML(BinaryData::menu_xml, BinaryData::menu_xmlSize);
			auto state = juce::ValueTree::fromXml(*xml.get());
			menu = std::make_unique<Menu>(p, utils, state);
			parentComp.addAndMakeVisible(menu.get());
			menu->setBounds(menuBounds);
		}
		else
			menu.reset(nullptr);

		openButton.repaint();
	}
}