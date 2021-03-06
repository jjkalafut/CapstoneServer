//#include "rtpsession.h"
//#include "rtpudpv4transmitter.h"
#include "rtpipv4address.h"
//#include "rtpsessionparams.h"
//#include "rtperrors.h"
//#include "rtppacket.h"
#ifndef WIN32
	#include <netinet/in.h>
	#include <arpa/inet.h>
#else
	#include <winsock2.h>
#endif // WIN32

//#include "timer.h"

#pragma once
namespace Project1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm()
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
		void pollPackets();
		void set_address( UINT addr);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	protected:
	private: System::Windows::Forms::ListBox^  Clients;
	private: System::Windows::Forms::ListBox^  Devices;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  importSettingsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  exportSettingsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  startServerToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  stopServerToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  toolsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  getHelpToolStripMenuItem;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;


	private: System::Windows::Forms::ToolStripMenuItem^  setAddressToolStripMenuItem;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->Clients = (gcnew System::Windows::Forms::ListBox());
			this->Devices = (gcnew System::Windows::Forms::ListBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->importSettingsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exportSettingsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->startServerToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->stopServerToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->setAddressToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->getHelpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 20));
			this->label1->Location = System::Drawing::Point(28, 38);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(273, 31);
			this->label1->TabIndex = 0;
			this->label1->Text = L"PandaServer - Status";
			// 
			// Clients
			// 
			this->Clients->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Clients->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->Clients->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			this->Clients->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(192)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->Clients->FormattingEnabled = true;
			this->Clients->ItemHeight = 20;
			this->Clients->Location = System::Drawing::Point(36, 88);
			this->Clients->Name = L"Clients";
			this->Clients->Size = System::Drawing::Size(383, 200);
			this->Clients->TabIndex = 1;
			this->Clients->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::Clients_SelectedIndexChanged);
			// 
			// Devices
			// 
			this->Devices->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Devices->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->Devices->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			this->Devices->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(192)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->Devices->FormattingEnabled = true;
			this->Devices->ItemHeight = 20;
			//this->Devices->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"Living Room \0", L"Hallway \0", L"Bedroom \0" });
			this->Devices->Location = System::Drawing::Point(535, 88);
			this->Devices->Name = L"Devices";
			this->Devices->Size = System::Drawing::Size(388, 200);
			this->Devices->TabIndex = 2;
			// 
			// menuStrip1
			// 
			this->menuStrip1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->fileToolStripMenuItem,
					this->helpToolStripMenuItem, this->toolsToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(962, 24);
			this->menuStrip1->TabIndex = 3;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->importSettingsToolStripMenuItem,
					this->exportSettingsToolStripMenuItem, this->exitToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// importSettingsToolStripMenuItem
			// 
			this->importSettingsToolStripMenuItem->Name = L"importSettingsToolStripMenuItem";
			this->importSettingsToolStripMenuItem->Size = System::Drawing::Size(155, 22);
			this->importSettingsToolStripMenuItem->Text = L"Import Settings";
			// 
			// exportSettingsToolStripMenuItem
			// 
			this->exportSettingsToolStripMenuItem->Name = L"exportSettingsToolStripMenuItem";
			this->exportSettingsToolStripMenuItem->Size = System::Drawing::Size(155, 22);
			this->exportSettingsToolStripMenuItem->Text = L"Export Settings";
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(155, 22);
			this->exitToolStripMenuItem->Text = L"Exit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::exitToolStripMenuItem_Click);
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->startServerToolStripMenuItem,
					this->stopServerToolStripMenuItem, this->setAddressToolStripMenuItem
			});
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(48, 20);
			this->helpToolStripMenuItem->Text = L"Tools";
			// 
			// startServerToolStripMenuItem
			// 
			this->startServerToolStripMenuItem->Name = L"startServerToolStripMenuItem";
			this->startServerToolStripMenuItem->Size = System::Drawing::Size(135, 22);
			this->startServerToolStripMenuItem->Text = L"Start Server";
			this->startServerToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::startServerToolStripMenuItem_Click);
			// 
			// stopServerToolStripMenuItem
			// 
			this->stopServerToolStripMenuItem->Name = L"stopServerToolStripMenuItem";
			this->stopServerToolStripMenuItem->Size = System::Drawing::Size(135, 22);
			this->stopServerToolStripMenuItem->Text = L"Stop Server";
			this->stopServerToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::stopServerToolStripMenuItem_Click);
			// 
			// setAddressToolStripMenuItem
			// 
			this->setAddressToolStripMenuItem->Name = L"setAddressToolStripMenuItem";
			this->setAddressToolStripMenuItem->Size = System::Drawing::Size(135, 22);
			this->setAddressToolStripMenuItem->Text = L"Set Address";
			this->setAddressToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::setAddressToolStripMenuItem_Click);
			// 
			// toolsToolStripMenuItem
			// 
			this->toolsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->getHelpToolStripMenuItem });
			this->toolsToolStripMenuItem->Name = L"toolsToolStripMenuItem";
			this->toolsToolStripMenuItem->Size = System::Drawing::Size(44, 20);
			this->toolsToolStripMenuItem->Text = L"Help";
			// 
			// getHelpToolStripMenuItem
			// 
			this->getHelpToolStripMenuItem->Name = L"getHelpToolStripMenuItem";
			this->getHelpToolStripMenuItem->Size = System::Drawing::Size(120, 22);
			this->getHelpToolStripMenuItem->Text = L"Get Help";
			// 
			// pictureBox1
			// 
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(336, 32);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(100, 50);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pictureBox1->TabIndex = 4;
			this->pictureBox1->TabStop = false;
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::Color::Black;
			this->button1->FlatAppearance->BorderColor = System::Drawing::Color::Lime;
			this->button1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button1->Location = System::Drawing::Point(535, 302);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(150, 35);
			this->button1->TabIndex = 5;
			this->button1->Text = L"Configure";
			this->button1->UseVisualStyleBackColor = false;
			// 
			// button2
			// 
			this->button2->BackColor = System::Drawing::Color::Black;
			this->button2->FlatAppearance->BorderColor = System::Drawing::Color::Lime;
			this->button2->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button2->Location = System::Drawing::Point(36, 302);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(150, 35);
			this->button2->TabIndex = 6;
			this->button2->Text = L"Configure";
			this->button2->UseVisualStyleBackColor = false;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14));
			this->label2->Location = System::Drawing::Point(569, 45);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(131, 24);
			this->label2->TabIndex = 9;
			this->label2->Text = L"My IP Address";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(747, 49);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(166, 20);
			this->textBox1->TabIndex = 10;
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox1_TextChanged);
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::timer1_Tick);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::Black;
			this->ClientSize = System::Drawing::Size(962, 370);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->Devices);
			this->Controls->Add(this->Clients);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->menuStrip1);
			this->ForeColor = System::Drawing::Color::Lime;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MyForm";
			this->Text = L"PandaServer";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void Clients_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
		//send_packet();
	}

	private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		this->Close();
	}

	private: System::Void setAddressToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		
	}
	private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		IntPtr ptrToNativeString = Marshal::StringToHGlobalAnsi( this->textBox1->Text );
		UINT destip;
		//printf(static_cast<char*>(ptrToNativeString.ToPointer()));
		//printf("\n");
		inet_pton(AF_INET, static_cast<char*>(ptrToNativeString.ToPointer()), &destip);
		destip = ntohl(destip);
		set_address(destip);
	}
	private: System::Void startServerToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		this->timer1->Interval = 2;
		this->timer1->Start();
	}
	private: System::Void stopServerToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		this->timer1->Stop();
	}
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
		pollPackets();
	}
};
}

