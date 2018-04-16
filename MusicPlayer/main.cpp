#include <iostream>
#include <bass.h>
#include <string>
#include "GetFileName.h"

#include "ArgsAnalysiser.h"
#include <SFGUI\SFGUI.hpp>
#include <SFGUI\Widget.hpp>
#include <SFGUI\Label.hpp>
#include <SFGUI\Window.hpp>
#include <SFGUI\Box.hpp>
#include <SFGUI\Button.hpp>
#include <SFGUI\Desktop.hpp>
#include <SFGUI\Renderer.hpp>
#include <SFGUI\Context.hpp>
#include <SFGUI\ScrolledWindow.hpp>
#include <SFGUI\Scale.hpp>
#include <SFGUI\Scrollbar.hpp>
#include <SFGUI\Adjustment.hpp>
#include <SFGUI\Canvas.hpp>
#include <SFGUI\RendererViewport.hpp>
#include <SFGUI\CheckButton.hpp>
#include <SFGUI\ComboBox.hpp>

#include "FFT1_View.h"
#include "FFT2_View.h"
#include "MusicPlayer.h"
#include "Pumper.h"

#include <SFML\Graphics.hpp>
#include <stack>
#include <thread>
#include <mutex>


#define FFT_DATA_SIZE 128

float FFT_DATA[FFT_DATA_SIZE];

std::stack<std::shared_ptr<std::vector<MMFile>>> file_root;

sf::Clock clock_for_scale;

bool need_update_list = false;

bool isPressed_for_scale = false;

bool enable3DSurround = false;

bool isDrawMs = false;

#define FFT_MODE_NUM 2

int fft_view_mode = 0; 
fv::FFT_View* fft_views[FFT_MODE_NUM];

#define POS_MAX_LEN 480

BASS_3DVECTOR pos[POS_MAX_LEN];
int pos_i = 0;


HFX fx[10] = { -1 };
sf::Text text_ms;

//std::mutex load_file_name_mutex;

void cleanFx();
inline void fill_music(fv::MusicPlayer *player, std::shared_ptr<sfg::Box> scrolled_window_box, fv::Pumper *pumper);
void loadAllCharGlyph();
void init_3D_pos();
void destroy();
void switch_fft_mode(sfg::Window &draw_fft_window);


int main(int argc,char **argv)
{	
	std::tuple<int, int, const char*,bool> args_tup = ArgsAnalysiser::analysis(argc, argv);

	fv::MusicPlayer player(std::get<3>(args_tup));

	fv::Pumper pumper(file_root, player);
	pumper.setFillMusicFunc([] {
		need_update_list = true;
	});
	
	if (std::get<3>(args_tup))
	{
		init_3D_pos();
	}

	sf::RenderWindow render_window(sf::VideoMode(std::get<0>(args_tup), std::get<1>(args_tup)), "MusicPlayer");



	render_window.setVerticalSyncEnabled(true);
	render_window.resetGLStates();

	// Create an SFGUI. This is required before doing anything with SFGUI.
	sfg::SFGUI sfgui;

	sfg::Context::Get().GetDefaultEngine().LoadThemeFromFile(".\\..\\..\\example.theme");
	

	auto v = std::make_shared<std::vector<MMFile>>();
	
	GetFileName::getFileNameA(*v, std::get<2>(args_tup));

	
	file_root.push(v);
	
	loadAllCharGlyph();

	

	sf::Texture texture_bg;
	texture_bg.loadFromFile(".\\..\\..\\bg.jpg");

	sf::Sprite bg_sprite;
	bg_sprite.setTexture(texture_bg,true);
	bg_sprite.setPosition(0.f, 0.f);

	auto window = sfg::Window::Create();
	window->SetTitle("list");

	auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 6.0f);
	auto scrolled_window_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

	auto scrolledwindow = sfg::ScrolledWindow::Create();
	
	scrolledwindow->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC);

	// Add the ScrolledWindow box to the ScrolledWindow
	// and create a new viewport automatically.
	scrolledwindow->AddWithViewport(scrolled_window_box);

	// Always remember to set the minimum size of a ScrolledWindow.
	scrolledwindow->SetRequisition(sf::Vector2f(300.f, 400.f));

	fill_music(&player, scrolled_window_box,&pumper);

	auto btn_play = sfg::Button::Create(L"播放");
	btn_play->GetSignal(sfg::Widget::OnLeftClick).Connect([&player] {
		player.play();
	});

	auto btn_pause = sfg::Button::Create(L"暂停");
	btn_pause->GetSignal(sfg::Widget::OnLeftClick).Connect([&player] {
		player.pause();
	});
		
	auto btn_stop = sfg::Button::Create(L"停止");
	btn_stop->GetSignal(sfg::Widget::OnLeftClick).Connect([&player] {
		player.stop();
	});

	auto btn_back = sfg::Button::Create(L"返回");
	btn_back->GetSignal(sfg::Widget::OnLeftClick).Connect([&player,scrolled_window_box] {
		if (file_root.size() > 1)
		{
			file_root.pop();
			need_update_list = true;
		}
	});

	auto btn_3d = sfg::CheckButton::Create(L"3D环绕");
	btn_3d->GetSignal(sfg::CheckButton::OnToggle).Connect(
		[btn_3d,&player] {
			
			if (player.isEnable3D() && player.isSupport3D())
			{
				enable3DSurround = btn_3d->IsActive();
				if (!enable3DSurround)
				{
					BASS_3DVECTOR p{ 0,0,0 };
					BASS_3DVECTOR vel{ 0 - pos[pos_i].x,0,0 - pos[pos_i].z };

					BASS_ChannelSet3DPosition(player.getChan(), &p, NULL, &vel);
					BASS_Apply3D();
				}
			}
			else
			{
				btn_3d->SetActive(false);
			}
			
		}
	);

	auto button_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 2.0f);

	button_box->Pack(btn_play,true,true);
	button_box->Pack(btn_pause, true, true);
	button_box->Pack(btn_stop,true,true);
	button_box->Pack(btn_back, true, true);
	

	auto scrollbar = sfg::Scrollbar::Create();
	scrollbar->SetRange(.0f, 100.f);

	auto scale = sfg::Scale::Create();
	scale->SetAdjustment(scrollbar->GetAdjustment());
	scale->SetRequisition(sf::Vector2f(50.0f,20.0f));

	scale->GetSignal(sfg::Widget::OnMouseLeftPress).Connect([] {
		isPressed_for_scale = true;
	});

	scale->GetSignal(sfg::Widget::OnMouseLeftRelease).Connect([scale,&player]
	{
		if (!player.isOff())
		{
			QWORD r = (player.getLength() / 100) * scale->GetValue();
			//printf("++++++++++%ld++++++++++\n",r);
			player.setPosition(r, BASS_POS_BYTE);
			//printf("-----------------------\n"); 
			clock_for_scale.restart();
		}
		isPressed_for_scale = false;
	});
	//scale->GetAdjustment()->GetSignal(sfg::Adjustment::OnChange).Connect();

	auto pump_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 2.0f);

	auto pump_mode_combobox = sfg::ComboBox::Create();

	pump_mode_combobox->AppendItem(L"None");
	pump_mode_combobox->AppendItem(L"Loop");
	pump_mode_combobox->AppendItem(L"Rand");

	pump_mode_combobox->GetSignal(sfg::ComboBox::OnSelect).Connect([pump_mode_combobox,&pumper] {
		switch (pump_mode_combobox->GetSelectedItem())
		{
		case 0:
			pumper.setMode(fv::Pumper::NONE);
			break;
		case 1:
			pumper.setMode(fv::Pumper::LOOP);
			break;
		case 2:
			pumper.setMode(fv::Pumper::RAND);
			break;
		}
	});

	auto btn_pump_dir = sfg::CheckButton::Create(L"自动展开文件夹");

	btn_pump_dir->GetSignal(sfg::CheckButton::OnToggle).Connect([btn_pump_dir, &pumper] {
		pumper.setPumpDir(btn_pump_dir->IsActive());
	});

	pump_box->Pack(pump_mode_combobox);
	pump_box->Pack(btn_pump_dir);
	pump_box->Pack(btn_3d);


	auto volumebar = sfg::Scrollbar::Create();
	volumebar->SetRange(.0f, 100.f);

	auto volumeScale = sfg::Scale::Create();
	volumeScale->SetAdjustment(volumebar->GetAdjustment());
	volumeScale->SetRequisition(sf::Vector2f(50.0f, 20.0f));


	volumeScale->SetValue(100.f);

	volumeScale->GetSignal(sfg::Widget::OnMouseLeftRelease).Connect([volumeScale] {
		BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, volumeScale->GetValue() * 100);
		//BASS_SetVolume( / 100.0f);
	});


	box->Pack(scale,false);
	box->Pack(button_box,false,true);
	box->Pack(pump_box, false);
	box->Pack(volumeScale, false);
	box->Pack(scrolledwindow);
	


	auto draw_fft_window = sfg::Window::Create();
	draw_fft_window->SetTitle("draw fft window");
	draw_fft_window->SetRequisition(sf::Vector2f(800.f, 700.0f));
	draw_fft_window->SetPosition(sf::Vector2f(400.f, 10.0f)); 
	

	auto canvas = sfg::Canvas::Create();

	fft_views[fft_view_mode] = new fv::FFT1_View(FFT_DATA,FFT_DATA_SIZE);

	fft_views[fft_view_mode]->setPosition(draw_fft_window->GetRequisition().x  / 2,
		draw_fft_window->GetRequisition().y  / 2
		);
	/*
	sf::RectangleShape level_low;
	level_low.setPosition((draw_fft_window->GetRequisition().x - 9) / 2.0f, 10);
	level_low.setSize(sf::Vector2f((draw_fft_window->GetRequisition().x - 28) / 2.0f, 20.f));
	level_low.setFillColor(sf::Color(0, 255, 0));
	level_low.setOutlineColor(sf::Color(138, 43, 226));
	level_low.setOutlineThickness(2.0);

	level_low.setOrigin(sf::Vector2f(level_low.getLocalBounds().width - 2.0f, 0.0f));

	sf::RectangleShape level_hig;
	level_hig.setPosition((draw_fft_window->GetRequisition().x - 8) / 2.0f, 10);
	level_hig.setSize(sf::Vector2f((draw_fft_window->GetRequisition().x - 28) / 2.0f, 20.f));
	level_hig.setFillColor(sf::Color(0, 255, 0));
	level_hig.setOutlineColor(sf::Color(138, 43, 226));
	level_hig.setOutlineThickness(2.0);*/

	
	draw_fft_window->Add(canvas);

	window->SetRequisition(sf::Vector2f(460.f, 400.f));
	window->Add(box);


	

	sfg::Desktop desktop;
	
	desktop.Add(window);
	desktop.Add(draw_fft_window);

	// Main loop!
	sf::Event event;
	sf::Clock clock;
	
	//bool f = false;
	//int i = 0;

	while (render_window.isOpen()) {
	
		clock.restart();

		if (need_update_list)
		{
			//std::thread t1(fill_music, &player, scrolled_window_box);
			//t1.detach();
			fill_music(&player, scrolled_window_box,&pumper);
			need_update_list = false;
		}

		if (clock_for_scale.getElapsedTime() >= sf::seconds(1))
		{
			if (!isPressed_for_scale && !player.isOff() && player.getActive() == BASS_ACTIVE_PLAYING)
			{
				QWORD pos = player.getPosition(BASS_POS_BYTE);
				scale->SetValue(((double)pos / (double)player.getLength()) * 100);
			}
			clock_for_scale.restart();
		}

		// Event processing.
		while (render_window.pollEvent(event)) {
			desktop.HandleEvent(event);

			// If window is about to be closed, leave program.
			if (event.type == sf::Event::Closed) {
				return 0;
			}
			if (event.type == sf::Event::KeyReleased) {
				switch (event.key.code)
				{
				case sf::Keyboard::D:
					isDrawMs = !isDrawMs;
					break;
				case sf::Keyboard::H:
					switch_fft_mode(*draw_fft_window);
					break;
				default:
					break;
				}
			}

			/*if (event.type == sf::Event::KeyReleased)
			{
				if (!player.isOff())
				{
					for (int i = 0; i < 10; i++)
					{
						if (event.key.code == i)
						{
							if (fx[i] == -1)
							{
								fx[i] = BASS_ChannelSetFX(player.getChan(), BASS_FX_DX8_CHORUS + i, i);
							}
							else
							{
								if (BASS_ChannelRemoveFX(player.getChan(), fx[i]))
								{
									fx[i] = -1;
								}
							}
						}
					}
				}
			}*/
		}

		

		//if (clock.getElapsedTime() >= sf::milliseconds(32))
		{
			
			if (!player.isOff() && player.getActive() == BASS_ACTIVE_PLAYING)
			{
				float l = 0.5f, h = 0.5f;
				DWORD level = player.getLevel();
				for (int a = 27204; a > 200; a = a * 2 / 3){
					if (LOWORD(level) >= a) l += 0.05f;
				}
				for (int a = 210; a<32768; a = a * 3 / 2) {
					if (LOWORD(level) >= a) h += 0.05f;
				}

				//printf("%d , %d\n", LOWORD(level), HIWORD(level));
				//level_low.setScale(sf::Vector2f( l , 1.0f));
				//level_hig.setScale(sf::Vector2f(h, 1.0f));

				player.getData(FFT_DATA, BASS_DATA_FFT256);
				fft_views[fft_view_mode]->updatePoint(l,h);
				
			}

			if (enable3DSurround && !player.isOff() && player.getActive() == BASS_ACTIVE_PLAYING)
			{
				BASS_3DVECTOR vel;
				if (pos_i == 0)
				{
					vel.x = pos[POS_MAX_LEN - 1].x - pos[0].x;
					vel.y = 0;
					vel.z = pos[POS_MAX_LEN - 1].z - pos[0].z;
				}
				else {
					vel.x = pos[pos_i - 1].x - pos[pos_i].x;
					vel.y = 0;
					vel.z = pos[pos_i - 1].z - pos[pos_i].z;
				}

				BASS_ChannelSet3DPosition(player.getChan(), &pos[pos_i], NULL, &vel);
				BASS_Apply3D();

				//printf("%lf, %lf, %lf   %lf, %lf, %lf\n", pos[pos_i].x, pos[pos_i].y, pos[pos_i].z, vel.x, vel.y, vel.z);

				++pos_i;
				if (pos_i == POS_MAX_LEN)
				{
					pos_i = 0;
				}
			}

			if (player.getActive() == BASS_ACTIVE_STOPPED)
			{
				pumper.pump();
			}

			//std::lock_guard<std::mutex> lock(load_file_name_mutex);
			desktop.Update(clock.getElapsedTime().asSeconds());
			clock.restart();
		}

		
		// Update SFGUI with elapsed seconds since last call.
		render_window.clear();

		canvas->Bind();
		canvas->Clear(sf::Color(112,128,144,50));
	
		canvas->Draw(*(fft_views[fft_view_mode]));
		//canvas->Draw(level_low);
		//canvas->Draw(level_hig);
		canvas->Display();
		canvas->Unbind();
		
		// Rendering.
		//render_window.setActive(true);
		render_window.draw(bg_sprite);
		
		if (isDrawMs)
		{
			render_window.draw(text_ms);
		}
		
		
		
		sfgui.Display(render_window);
		render_window.display();
		
		
		//display sleep time
		int t = clock.getElapsedTime().asMilliseconds();
		
		
		if(isDrawMs)
		{ 
			char buf[10] = { 0 };
			sprintf_s(buf, 10, "%d ms", t);
			text_ms.setString(buf);
		}
		if ((t = 16 - t) > 0)
		{
			std::this_thread::sleep_for(std::chrono::duration<int,std::milli>(t));
		}
	}

	destroy();

	return EXIT_SUCCESS;
}

void init_3D_pos()
{
	float pi = 3.141592654f;
	float m_radius = 2;
	for (int i = 0; i < POS_MAX_LEN; ++i)
	{
		float angle = i * 2 * pi / POS_MAX_LEN - pi / 2;
		float x = std::cos(angle) * m_radius;
		float z = std::sin(angle) * m_radius;

		pos[i] = BASS_3DVECTOR(x, 0.0, z);
	}
}

void fill_music(fv::MusicPlayer *player,std::shared_ptr<sfg::Box> scrolled_window_box, fv::Pumper *pumper)
{
	
	//std::lock_guard<std::mutex> lock(load_file_name_mutex);
	scrolled_window_box->RemoveAll();
	auto v = file_root.top();
	

	for (int i = 0; i < v->size(); i++)
	{
		
		auto btn = sfg::Button::Create((*v)[i].getName());

		btn->GetSignal(sfg::Widget::OnLeftClick).Connect(
			[v, i, player,pumper]
		{
			if ((*v)[i].getType() == 16)
			{
				auto temp_v = std::make_shared<std::vector<MMFile>>();
				GetFileName::getFileNameW(*temp_v, (*v)[i].getAbsolutePath());
				
				//std::lock_guard<std::mutex> lock(load_file_name_mutex);
				file_root.push(temp_v);
				need_update_list = true;
				pumper->setIndex(0);
			}
			else {
				//cleanFx();
				player->playStream((*v)[i]);
				pumper->setIndex(i + 1);
			}
		});
		scrolled_window_box->PackEnd(btn);
	}
}

void loadAllCharGlyph()
{
	auto a = sfg::Button::Create();

	const auto& font_name = sfg::Context::GetDefaultEngine().GetProperty<std::string>("FontName", a);
	auto font_size = sfg::Context::GetDefaultEngine().GetProperty<unsigned int>("FontSize", a);
	const auto& font = sfg::Context::GetDefaultEngine().GetResourceManager().GetFont(font_name);

	//init text for ms
	{
		text_ms.setFont(*font);
		text_ms.setPosition(0, 0);
		text_ms.setFillColor(sf::Color::Green);
	}


	std::stack<std::shared_ptr<std::vector<MMFile>>> root = file_root;

	std::shared_ptr<std::vector<MMFile>> n;
	for (;;)
	{
		if (root.empty())
		{
			break;
		}
		else
		{
			n = root.top();
			root.pop();
		}
		for (MMFile &f : *n)
		{
			int len = lstrlenW(f.getName());
			for (int i = 0; i < len; i++)
			{
				font->getGlyph(f.getName()[i], font_size, false);
			}
			if (f.getType() == 16) 
			{
				std::shared_ptr<std::vector<MMFile>> temp = std::make_shared<std::vector<MMFile>>();
				GetFileName::getFileNameW(*temp, f.getAbsolutePath());
				root.push(temp);
			}
		}
	}
}

void cleanFx()
{
	for (int i = 0; i < 10; i++)
	{
		fx[i] = -1;
	}
}

void destroy()
{
	for (int i = 0; i < FFT_MODE_NUM; i++)
	{
		if (fft_views[i])
		{
			delete fft_views[i];
		}
	}
}

void switch_fft_mode(sfg::Window &draw_fft_window)
{
	fft_view_mode++;
	if (fft_view_mode >= FFT_MODE_NUM)
	{
		fft_view_mode = 0;
	}
	if (fft_views[fft_view_mode] == nullptr)
	{
		switch (fft_view_mode)
		{
		case 0:
			fft_views[fft_view_mode] = new fv::FFT1_View(FFT_DATA, FFT_DATA_SIZE);
			break;
		case 1:
			fft_views[fft_view_mode] = new fv::FFT2_View(FFT_DATA, FFT_DATA_SIZE);
			break;
		}
		fft_views[fft_view_mode]->setPosition(draw_fft_window.GetRequisition().x / 2,
			draw_fft_window.GetRequisition().y / 2
		);
	}
}

/*

HSTREAM chan = BASS_StreamCreateFile(FALSE, "D:\\Music\\Big\\MKJ - Time.mp3", 0, 0, BASS_SAMPLE_FLOAT);
printf("D:\\Music\\Big\\CG - 鏂囩埍.mp3");
if (chan == 0)
{
int errorCode = BASS_ErrorGetCode();
printf("errorCode = %d    \n", errorCode);
return 1;
}
printf("%d\n", chan);

BASS_ChannelPlay(chan, FALSE);

while(BASS_ChannelIsActive(chan))
{
sf::sleep(sf::milliseconds(16));
}
system("pause");*/

