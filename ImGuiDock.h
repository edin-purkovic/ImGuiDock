#pragma once

    // wrap your platform code around this.. 
    class GuiWindow
		{
		public:
			virtual ~GuiWindow() = default;

			// forbid copying
			GuiWindow(GuiWindow const &) = delete;
			GuiWindow & operator=(GuiWindow const &) = delete;

			virtual bool create( std::string title, uint32_t posx, uint32_t posy, uint32_t width, uint32_t height, uint32_t flags = fShow) = 0;
			virtual void setPosition(int x, int y) = 0;
			virtual void setSize(int width, int height) = 0;
			virtual void startDrag() = 0;
			virtual bool draging() = 0;
			virtual void stopDrag() = 0;
			virtual void destroy() = 0;

			virtual void getMouseScreenPosition(int &x, int &y) = 0;
			virtual void getWindowPosition(int &x, int &y) = 0;
			
			/* // ugly and broken.. just a regular repaint/update func
			virtual void onPaint(){
			
        // i call this here to check does it need to redock or is it dragging.. should move to Dockspace as a function.. 
        
				bool draging = false;
        if (dockspace.m_container.splits[0] && dockspace.m_container.splits[0]->activeDock)
        {
          bool mouseleft = GetKeyState(VK_LBUTTON) & 0x8000;
          if (dockspace.m_container.splits[0]->activeDock->draging && mouseleft)
          {
            //setTransparency(.7);
            startDrag();
            draging = true;
          }
          else
          {
            if (dockspace.m_container.splits[0]->activeDock->draging)
            {
              if(dockspace.m_container.splits[0]->activeDock->redockTo)
              {
                dockspace.m_container.splits[0]->activeDock->container = nullptr;
                dockspace.m_container.splits[0]->activeDock->redockFrom->dockWith(
                  dockspace.m_container.splits[0]->activeDock,
                  dockspace.m_container.splits[0]->activeDock->redockTo,
                  dockspace.m_container.splits[0]->activeDock->redockSlot,0,true);
                dockspace.m_container.splits[0]->activeDock->redockTo = nullptr;
               // m_closeWindow = true; --> close this window
                dockspace.m_container.splits[0]->activeDock->redockFromWindow->m_imGuiContext->IO.MouseDown[0] = false;
                dockspace.m_container.splits[0]->activeDock->redockFromWindow->m_imGuiContext->IO.MouseDelta = ImVec2();
                return false;
              }
              else
              {
                setPosition(m_posx, m_posy - (m_imGuiMenuBar ? 32 + 21 - 4 : 32 - 4));
                setSize(m_width, m_height + (m_imGuiMenuBar ? 32 + 21 : 32));
                setTransparency(1);
              }
            }
            dockspace.m_container.splits[0]->activeDock->draging = false;
            draging = false;
          }
          
          //rest of the code...
          //if(draging == false)
          //  imgui_drawTitleBar(draging);
          // your code..

        }
			}*/
  };
 
namespace ImGuiDock
{
	enum class DockSlot { Left, Right, Top, Bottom, Tab, None };
	
	struct Dock;
	class Dockspace;

	struct Container
	{
		Container *splits[2]{ nullptr, nullptr };
		Container *parent = nullptr;
		Dock *activeDock = nullptr;
		
		std::vector<Dock*>docks;

		bool verticalSplit = false;
		bool alwaysAutoResize = true;
		float size = 0;
	};

	struct Dock
	{
		Dock *initialize(const char *dtitle,  bool dcloseButton, ImVec2 dminSize, std::function<void(ImVec2)> ddrawFunction)
		{
			title = dtitle;
			closeButton = dcloseButton;
			minSize = dminSize;
			drawFunction = ddrawFunction;
			return this;
		};

		//Container *parent = nullptr;
		Container *container = nullptr;
		Dockspace *redockFrom = nullptr;
		GuiWindow *redockFromWindow = nullptr;
		Dock *redockTo = nullptr;

		const char *title;
		DockSlot dockSlot = DockSlot::Tab;
		DockSlot redockSlot = DockSlot::None;
		bool closeButton = true;
		bool undockable = false;
		bool draging = false;
		ImVec2 lastSize;
		ImVec2 minSize;

		std::function<void(ImVec2)> drawFunction;
		std::function<bool(void)> onCloseFunction;
	};
	
	typedef std::vector<GuiWindow*> GuiWindowList;
	
	class Dockspace
	{
	public:
		Dockspace(GuiWindowList *windowListPtr, GuiWindow *owner);
		~Dockspace();

		bool dock(Dock *dock, DockSlot dockSlot, float size = 0, bool active = false);
		bool dockWith(Dock *dock, Dock *dockTo, DockSlot dockSlot, float size = 0, bool active = false);
		bool undock(Dock *dock);
		
		void updateAndDraw(ImVec2 size);
		void clear();

		std::vector<Dock*>m_docks;

		Container m_container;
		std::vector<Container*>m_containers;

	protected:
		friend class GuiWindow;

		DockSlot _renderDockSlotPreview(int mx, int my, ImVec2 cPos, ImVec2 cSize);
		void _renderTabBar(Container *container, ImVec2 size, ImVec2 cursorPos);
		bool _getMinSize(Container *container, ImVec2 *min);
		GuiWindow *_isAnyWindowDragging();

		enum DockToAction
		{
			eUndock, eDrag, eClose, eNull
		};

		Dock *m_currentDockTo = nullptr;
		DockToAction m_currentDockToAction = eNull;
		GuiWindowList *m_windowListPtr;
		GuiWindow *m_owner;
	};
};
