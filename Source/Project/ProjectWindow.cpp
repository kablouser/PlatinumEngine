//
// Created by Matt on 05/04/2022.
//

#include <Project/ProjectWindow.h>
#include <ComponentComposition/GameObject.h>
#include <ComponentComposition/Transform.h>
#include <ComponentComposition/MeshRender.h>
#include <Application.h>
#include <AssetDatabase/AssetHelper.h>
#include <SceneManager/SceneWithTemplates.h>

using namespace PlatinumEngine;

ProjectWindow::ProjectWindow() = default;

void ProjectWindow::ShowGUIWindow(bool* isOpen)
{
	if (ImGui::Begin(ICON_FA_FOLDER " Project Window"))
	{
		//Top Bar for Project Window
		static char assetSelectorBuffer[128];
		ImGui::InputText("##FilterAssets", assetSelectorBuffer, IM_ARRAYSIZE(assetSelectorBuffer));
		ImGui::SameLine();
		ImGui::Text("%s", ICON_FA_MAGNIFYING_GLASS);
		ImGui::SameLine();

		//Settings for Toggle (Preview Button)

		if(_isPreviewEnabled)
		{
			//Preview Button Settings
			ImGuiStyle style = ImGui::GetStyle();
			ImGui::PushID("Preview");
			ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_ButtonActive]);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, style.Colors[ImGuiCol_ButtonActive]);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, style.Colors[ImGuiCol_ButtonActive]);
			ImGui::Button("Preview");
			if (ImGui::IsItemClicked(0))
			{
				_isPreviewEnabled = false;
				_childWindowCount -= 1;
			}

			ImGui::PopStyleColor(3);
			ImGui::PopID();
		}
		else
		{
			if (ImGui::Button("Preview"))
			{
				_isPreviewEnabled = true;
				_childWindowCount += 1;
			}

		}

		//Child Windows
		if(ImGui::BeginChild("Project View",ImVec2(ImGui::GetWindowWidth()/_childWindowCount,0),true))
		{
			// If we don't have something to search for, use default assets folder
			_toFind = assetSelectorBuffer;
			if (!_toFind.empty())
			{
				// Recursively search for the target string in each directory
				for (std::filesystem::recursive_directory_iterator i(_parentFolder), end; i != end; ++i)
				{
					// Find search string in filename, also check if parent path contains search string to avoid adding twice
					if (i->path().filename().string().find(_toFind) != std::string::npos && (i->path().parent_path().string().find(_toFind) == std::string::npos))
					{
						ShowTreeNode(i->path());
					}
				}
			}
			else
			{
				ShowTreeNode(std::filesystem::path(_parentFolder));
			}
		}
		ImGui::EndChild();

		if(_isPreviewEnabled)
		{
			ImGui::SameLine();
			if(ImGui::BeginChild("Preview Window",ImVec2(ImGui::GetWindowWidth()/_childWindowCount,0),true))
			{
				if(!_previewFilePath.empty())
					ShowProjectWindowPreview(_previewFilePath);
			}
			ImGui::EndChild();
		}

	}

	ImGui::End();
}

/**
 * This is a recursive function that will show a tree structure from a given parent directory
 * @param dir the current directory level
 */
void ProjectWindow::ShowTreeNode(std::filesystem::path dir)
{
	// Common flags for all items in tree
	auto flags = ImGuiTreeNodeFlags_FramePadding;

	// If a directory, need to loop its sub-contents
	if (is_directory(dir))
	{
		// Hide label of node using "##" (this is so we can have fancy icons)
		bool isExpanded = ImGui::TreeNodeEx((std::string{"##"} + dir.filename().string()).c_str(), flags);

		bool isRemoved = false;

		//Right-click actions
		bool renameFolder = false;
		if(ImGui::BeginPopupContextItem())
		{
			if (ImGui::Selectable("Add Folder"))
			{
				std::string name = "New Folder";
				std::filesystem::path newFolderPath = std::filesystem::path(dir.string()+"/"+name);
				int i=2;
				while(exists(newFolderPath))
				{
					newFolderPath = std::filesystem::path(dir.string()+"/"+name+" ("+std::to_string(i)+")");
					i++;
				}
				std::filesystem::create_directory(newFolderPath);
			}
			if(ImGui::Selectable("Remove Folder"))
			{
				isRemoved = true;
			}
			if(_moveType!=MoveType::none)
			{
				if (ImGui::Selectable("Paste File"))
				{
					if (_moveType == MoveType::copy)
					{
						std::filesystem::path newPath = std::filesystem::path(
								dir.string() + "/" + _moveFilePath.filename().string());
						std::filesystem::copy_file(_moveFilePath, newPath);
						_moveType = MoveType::none;
					}
					if (_moveType == MoveType::cut)
					{
						std::filesystem::path newPath = std::filesystem::path(
								dir.string() + "/" + _moveFilePath.filename().string());
						std::filesystem::rename(_moveFilePath, newPath);
						_moveType = MoveType::none;
					}
				}
			}
			ImGui::EndPopup();
		}

		// Only show sub-contents if tree node is expanded
		if (isExpanded)
		{
			// Add open folder icon and name
			ImGui::SameLine();
			ImGui::Text("%s", (std::string{ICON_FA_FOLDER_OPEN} + " " + dir.filename().string()).c_str());

			//Drag-and-Drop actions
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RegularFilePathPayload"))
					DragDropMoveRegularFile(dir, payload);

				ImGui::EndDragDropTarget();
			}

			for (std::filesystem::directory_iterator i(dir), end; i != end; ++i)
			{
				ShowTreeNode(i->path());
			}

			ImGui::TreePop();
		}
		else
		{
			// Add closed folder icon and name
			ImGui::SameLine();
			ImGui::Text("%s", (std::string{ICON_FA_FOLDER} + " " + dir.filename().string()).c_str());
			//Drag-and-Drop actions
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RegularFilePathPayload"))
					DragDropMoveRegularFile(dir, payload);

				ImGui::EndDragDropTarget();
			}
		}

		//Remove it if flag is set to true
		if(isRemoved)
			std::filesystem::remove_all(dir.string());
	}
	else
	{
		// Do not want to add the asset database to the tree
		if (dir.filename() != _ignoreDatabaseName && dir.filename() != _ignoreMacFolderAttribFile)
		{
			// A path is a leaf of a tree (i.e. it cannot be expanded)
			ImGui::TreeNodeEx(dir.filename().string().c_str(), flags | ImGuiTreeNodeFlags_Leaf);

			//If we click on the file
			if(ImGui::IsItemClicked(0))
			{
				_previewFilePath = dir;
				_modelRotation = Maths::Vec3(0,0,0);
			}

			//Right-click actions
			if (ImGui::BeginPopupContextItem())
			{
				//Specific extension related functionalities
				if(dir.extension()==".obj")
				{
					if (ImGui::Selectable("Add Mesh"))
					{
						SavedReference<GameObject> go = Application::Instance->scene.AddGameObject(dir.stem().string());
						Application::Instance->scene.AddComponent<Transform>(go);
						Application::Instance->scene.AddComponent<MeshRender>(go);
						auto asset_Helper = AssetHelper::GetAsset<Mesh>(dir.string());
						if (std::get<0>(asset_Helper))
							go.DeRef()->GetComponent<MeshRender>().DeRef()->SetMesh(std::get<1>(asset_Helper));
						Application::Instance->sceneEditor.SetSelectedGameobject(go);
					}
					ImGui::Separator();
				}
				if(dir.extension()==".png")
				{
					SavedReference<GameObject> go = Application::Instance->sceneEditor.GetSelectedGameobject();
					if(go.DeRef() == nullptr)
						ImGui::CloseCurrentPopup();
					if (ImGui::Selectable("Add Texture"))
					{
						auto asset_Helper = AssetHelper::GetAsset<Texture>(dir.string());
						if (std::get<0>(asset_Helper))
						{
							go.DeRef()->GetComponent<MeshRender>().DeRef()->SetMaterial(std::get<1>(asset_Helper));
							go.DeRef()->GetComponent<MeshRender>().DeRef()->material.useTexture = true;
						}
					}
					if (ImGui::Selectable("Add Normal"))
					{
						auto asset_Helper = AssetHelper::GetAsset<Texture>(dir.string());
						if (std::get<0>(asset_Helper))
							go.DeRef()->GetComponent<MeshRender>().DeRef()->SetNormalMap(std::get<1>(asset_Helper));
					}
					ImGui::Separator();
				}
				if(dir.extension()==".wav")
				{
					SavedReference<GameObject> go = Application::Instance->sceneEditor.GetSelectedGameobject();
					if(go.DeRef() != nullptr)
					{
						if (ImGui::Selectable("Add Audio"))
						{
							auto asset_Helper = AssetHelper::GetAsset<AudioClip>(dir.string());
							if (std::get<0>(asset_Helper))
							{
								if (!go.DeRef()->GetComponent<AudioComponent>())
									Application::Instance->scene.AddComponent<AudioComponent>(go);
								go.DeRef()->GetComponent<AudioComponent>().DeRef()->audioClip = std::get<1>(
										asset_Helper);
							}
						}
					}
					ImGui::Separator();
				}

				//General file functionalities
				if (ImGui::Selectable("Copy File"))
				{
					_moveFilePath = dir;
					_moveType = MoveType::copy;
				}
				if (ImGui::Selectable("Cut File"))
				{
					_moveFilePath = dir;
					_moveType = MoveType::cut;
				}
				if (ImGui::Selectable("Delete File"))
				{
					std::filesystem::remove(dir);
				}
				ImGui::EndPopup();
			}

			ImGui::TreePop();

			//Drag-and-Drop actions
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				// Set payload to carry the filepath
				if(is_regular_file(dir))
				{
					ImGui::SetDragDropPayload("RegularFilePathPayload", dir.string().c_str(), dir.string().length());
					ImGui::TextUnformatted(dir.filename().string().c_str());
				}
				ImGui::EndDragDropSource();
			}

		}
	}
}

/**
 * This handles Drag-and-drop moving of files to different directories
 * @param dir the current directory level
 * @param payload the dragged payload
 */
void ProjectWindow::DragDropMoveRegularFile(std::filesystem::path dir, const ImGuiPayload* payload)
{
	//MAY CAUSE ISSUES WITH ASSET DATABASE
	char* payloadPointer = (char*)payload->Data;
	int size = payload->DataSize;
	std::string filePath = "";
	for(int i=0;i<size;i++)
		filePath+=*(payloadPointer+i);
	std::filesystem::path payloadPath = std::filesystem::path(filePath);
	if(payloadPath.parent_path()!=dir)
	{
		std::string name = std::filesystem::path(payloadPointer).stem().string();
		std::filesystem::rename(payloadPath.string(), dir.string()+"\\"+payloadPath.filename().string());
	}
}

/**
 * This handles file previews and displays relevant information about the file
 * @param filePath the current selected file to preview
 */
void ProjectWindow::ShowProjectWindowPreview(std::filesystem::path filePath)
{
	//Preview the image
	if(filePath.extension()==".png")
	{
		//Grab the texture from AssetDatabase (pre-loaded, so we don't need to load again here)
		SavedReference<Texture> image;
		auto asset_Helper = AssetHelper::GetAsset<Texture>(filePath.string());
		if (std::get<0>(asset_Helper))
		{
			image = std::get<1>(asset_Helper);
			ImGui::Image((void*)(intptr_t)image.DeRef()->GetOpenGLHandle(),
					ImVec2(_framebufferWidth, _framebufferHeight));
		}

		ImGui::Text("Type: IMAGE");

		std::string dim = std::to_string((int)image.DeRef()->width)+" x "+std::to_string((int)image.DeRef()->height);
		ImGui::Text("Dimensions: ");
		ImGui::SameLine();
		ImGui::Text("%s", dim.c_str());
	}

	//Preview the mesh
	else if(filePath.extension()==".obj")
	{
		//We can also define a subwindow to display a resizable framebuffer in future
		//auto targetSize = ImGui::GetContentRegionAvail();

		//Render the mesh
		RenderPreview(filePath);

		ImGui::Text("Type: MESH");
	}

	//Preview the audio
	else if(filePath.extension()==".wav")
	{
		//Just consider that audio files are of type clip (.wav files)
		//Just simple play/stop button to preview
		SavedReference<AudioClip> audioSample;
		auto asset_Helper = AssetHelper::GetAsset<AudioClip>(filePath.string());
		if (std::get<0>(asset_Helper))
		{
			audioSample = std::get<1>(asset_Helper);

			if (ImGui::Button("Play"))
			{
				if (Mix_Playing(-1) > 0)
					Mix_HaltChannel(-1);
				int channel = Mix_PlayChannel(-1, audioSample.DeRef()->chunk, 0);
			}
			ImGui::SameLine();
			if (ImGui::Button("Stop"))
			{
				if (Mix_Playing(-1) > 0)
					Mix_HaltChannel(-1);
			}
		}

		ImGui::Text("Type: AUDIO");
	}

	//Any other file
	else
	{
		//We just display general info (if any)
		ImGui::Text("Type: OTHER");
	}

	//Common info
	ImGui::Text("Name: ");
	ImGui::SameLine();
	ImGui::Text(filePath.filename().string().c_str());

	ImGui::Text("Size: ");
	ImGui::SameLine();
	ImGui::Text(FormatFileSize(file_size(filePath)).c_str());
}

/**
 * This handles filesize conversions so it outputs a filesize in human-readable format
 * @param size the size of file in bytes
 * @param precision how many decimal places to display (default: 2)
 * @return FileSize in human-readable format (default decimal point precision: 2)
 */
std::string ProjectWindow::FormatFileSize(uintmax_t size, int precision)
{
	int i = 0;
	const char *sizes[5] = { "B", "KB", "MB", "GB", "TB" };
	double dblByte = size;
	for (i = 0; i < 5 && size >= 1024; i++, size /= 1024)
		dblByte = size / 1024.f;

	std::stringstream stream;
	stream << std::fixed << std::setprecision(precision) << dblByte;
	return (stream.str()+" "+sizes[i]);
}

/**
 * This handles rendering of mesh into an image
 * @param filePath the path of the mesh
 */
void ProjectWindow::RenderPreview(std::filesystem::path filePath)
{
	_scene.Clear();
	// Create the texture of fixed size (No need for checks since we know that framebuffer will be of sufficient size)
	_renderTexture.Create(_framebufferWidth, _framebufferHeight);

	// bind framebuffer
	_renderTexture.Bind();

	// initiate setting before rendering
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, _framebufferWidth, _framebufferHeight);
	glClearColor(0.2784f, 0.2784f, 0.2784f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Start rendering (bind a shader)
	_renderer.Begin();

	// Get the mesh to be rendered
	SavedReference<Mesh> mesh;
	auto asset_Helper = AssetHelper::GetAsset<Mesh>(filePath.string());
	if (std::get<0>(asset_Helper))
	{
		mesh = std::get<1>(asset_Helper);

		// Setup shader
		ShaderInput _shaderInput;
		if (mesh)
		{
			_shaderInput.Clear();
			_shaderInput.Set(mesh.DeRef()->vertices, mesh.DeRef()->indices);
		}
		else
			_shaderInput.Clear();
		_shaderInput.Draw();
	}

	// Using a perspective camera
	_previewCamera.SetPerspectiveMatrix(60.f, _framebufferWidth/_framebufferHeight, 0.1f, 1000.f);

	// Set the rotation of the model
	Maths::Mat4 modelRotationMat;
	modelRotationMat.SetRotationMatrix(_modelRotation);

	// Setup renderer's settings
	_renderer.SetModelMatrix(modelRotationMat);
	_renderer.SetViewMatrix(_previewCamera.viewMatrix4);
	_renderer.SetProjectionMatrix(_previewCamera.projectionMatrix4);

	SavedReference<GameObject> go = _scene.AddGameObject("Temporary");
	_scene.AddComponent<LightComponent>(go);
	_scene.AddComponent<Transform>(go);
	std::vector<SavedReference<LightComponent>> lights;
	lights.push_back(go.DeRef()->GetComponent<LightComponent>());
	_renderer.SetupLights(lights);

	// End rendering (unbind a shader)
	_renderer.End();

	// unbind framebuffer
	_renderTexture.Unbind();

	// reset setting after rendering
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// display updated framebuffer
	ImGui::Image(_renderTexture.GetColorTexture().GetImGuiHandle(), ImVec2(_framebufferWidth,_framebufferHeight), ImVec2(0, 1), ImVec2(1, 0));

	// Setup this renderer's camera settings
	if(ImGui::IsItemHovered())
	{
		//Mouse Wheel changes zoom
		float wheelValue = ImGui::GetIO().MouseWheel;
		if (wheelValue != 0)
			_previewCamera.TranslationByKeyBoard(wheelValue,0);

		// Item's properties
		ImVec2 itemSize = ImGui::GetItemRectSize();
		ImVec2 itemPosMin = ImGui::GetItemRectMin();
		ImVec2 itemPosMax = ImGui::GetItemRectMax();

		//Calculate mouse position
		ImVec2 absMousePos = ImGui::GetMousePos();
		ImVec2 relMousePos = ImVec2(absMousePos.x-itemPosMin.x,absMousePos.y-itemPosMin.y);

		//Drawing overlay
		if(relMousePos.x<_framebufferWidth/4.f)
			ImGui::GetWindowDrawList()->AddRectFilled(itemPosMin, ImVec2(itemPosMin.x+_framebufferWidth/4.f,itemPosMin.y+itemSize.y), IM_COL32(255, 255, 255, 32));
		else if(relMousePos.x >_framebufferWidth*3.f/4.f)
			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(itemPosMin.x+_framebufferWidth*3.f/4.f,itemPosMin.y), itemPosMax, IM_COL32(255, 255, 255, 32));
		if(relMousePos.y<_framebufferHeight/4.f)
			ImGui::GetWindowDrawList()->AddRectFilled(itemPosMin, ImVec2(itemPosMin.x+itemSize.x,itemPosMin.y+_framebufferHeight/4.f), IM_COL32(255, 255, 255, 32));
		else if(relMousePos.y >_framebufferHeight*3.f/4.f)
			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(itemPosMin.x,itemPosMin.y+_framebufferHeight*3.f/4.f), itemPosMax, IM_COL32(255, 255, 255, 32));

		// Rotate the model
		if(ImGui::IsMouseDown(ImGuiMouseButton_Left))
		{
			//Calculate Mouse position
			if(relMousePos.x<_framebufferWidth/4.f)
				_modelRotation.y -= 0.05f;
			else if(relMousePos.x >_framebufferWidth*3.f/4.f)
			_modelRotation.y += 0.05f;

			if(relMousePos.y<_framebufferHeight/4.f)
				_modelRotation.x -= 0.05f;
			else if(relMousePos.y >_framebufferHeight*3.f/4.f)
				_modelRotation.x += 0.05f;
		}
	}

}