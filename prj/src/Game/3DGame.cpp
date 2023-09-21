//---------------------------------------------------------------
// �C���N���[�h
//---------------------------------------------------------------
#include "../external/hlslpp/include/hlsl++.h"
#include "../System/WinMain.h"
#include "../System/FileLoader/FileStream.h"
#include "../System/Input.h"
#include "../System/Class/Editor.h"
#include "../System/Class/Vector3.h"
#include "../System/Class/Effect.h"
#include "Class/CharacterBase.h"

#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_win32.h"
#include "../external/imgui/imgui_impl_dx11.h"

#include "Class/Stage.h"
#include "Class/Player.h"
#include "Class/Boss.h"
#include "Class/Camera.h"

#include <fstream>
#include <string>
#include <vector>
#include "../System/Class/Tutorial.h"

#include "3DGame.h"
#include "../System/Hit.h"


namespace
{
	//---------------------------------------------------------------
	// �񋓑�
	//---------------------------------------------------------------
	//! @brief �Q�[���̉�ʂ𑀍삷��񋓑�
	enum SCENE
	{
		Title,		//!< �^�C�g��
		Edit,		//!< �G�f�B�b�g
		Training,	//!< �g���[�j���O
		Movie,		//!< �G�̓o�ꃀ�[�r�[��
		Game,		//!< �Q�[��
		Finish,		//!< �I��
	};

	//! @brief ���f���f�[�^�̎Q�Ɨp
	enum MODELTYPE
	{
		PlayerData,		//!< �v���C���[
		BossData,		//!< �{�X
		StageData,		//!< �X�e�[�W
		TrainingData,	//!< �g���[�j���O�X�e�[�W
		TypeMax,
	};

	//! @brief �^�C�g����ʂ̃{�^���̕`�惂�[�h
	enum TITLETELOPTYPE	
	{
		PushButton,		//!< �{�^�����͑҂�
		SelectMode,		//!< ���[�h�I��҂�
		TutorialSwitch,	//!< �`���[�g���A���L���̑I��҂�
		VIEWTYPEMAX,	//!< �ő吔
	};

	//! @brief �`���[�g���A���̗L��
	enum TUTORIALSWITCH
	{
		On,
		Off,
		SWITCHMAX,
	};

	//---------------------------------------------------------------
	// �ϐ�
	//---------------------------------------------------------------
	//-------------------------------------
	// �Q�[���̑���Ɏg�p����ϐ�
	//-------------------------------------
	const float STAGERADIUS = 210.0f;		//!< �X�e�[�W�̈ړ��͈͂̔��a
	int		    nowScene;					//!< �Q�[���̃V�[���p�̕ϐ�
	int		    lockOnMakerImg;				//!< ���b�N�I�����̃}�[�J�[�̉摜�f�[�^
	bool	    isDebug;					//!< �f�o�b�O���[�h�̃t���O
	bool		isTutorial;					//!< �`���[�g���A���̗L��

	//-------------------------------------
	// �^�C�g����ʂ̑���Ŏg�p����ϐ�
	//-------------------------------------
	const int	ALPHAHMOVESPEED = 8;	//!< �����x�̑����̑��x
	const int	FADEVALUESPEED = 5;		//!< �t�F�[�h�A�E�g�̑��x
	int			titleTelopMode;			//!< �e���b�v�\�����[�h
	int			screenFadeValue;		//!< ��ʂ̃t�F�[�h�C���A�t�F�[�h�A�E�g�Ɏg�p����{�b�N�X�̓����l
	int			titleLogoImg;			//!< �^�C�g�����S�̉摜�f�[�^������
	int			alphaValue;				//!< �e���b�v��J�[�\���̓����x
	int			selectCursor;			//!< �^�C�g����ʂŎg�p����J�[�\���̔ԍ�
	bool		isViewingMode;			//!< ���[�h�I�����\������Ă��邩
	bool		isRisingAlphaValue;		//!< �����x�̑����t���O
	bool		isFadeIn  = false;		//!< �t�F�[�h�C���t���O
	bool		isFadeOut = false;		//!< �t�F�[�h�A�E�g�t���O

	//-------------------------------------
	// �V���h�E�}�b�v�Ɏg���ϐ�
	//-------------------------------------
	const VectorInt2 shadowSize = {1024,1024};		//!< �V���h�E�}�b�v�̃T�C�Y

	//-------------------------------------
	// �}�E�X�ł̉�ʑ���Ɏg�p����ϐ�
	//-------------------------------------
	Vector2 gameModeRange;		//!< �uGameMode�v������̑I��͈�
	Vector2 editModeRange;		//!< �uEditMode�v������̑I��͈�

	//-------------------------------------
	// �N���X�̃I�u�W�F�N�g
	//-------------------------------------
	Camera    gameCamera;	//!< �Q�[�����f���J����
	Stage     gameStage;	//!< �X�e�[�W�N���X
	Player    player;		//!< �v���C���[�N���X
	Boss      boss;			//!< �{�X�N���X
	Editor    editor;		//!< �G�f�B�^�[
	Tutorial  tutorial;		//!< �`���[�g���A��

	//-------------------------------------
	// �t�@�C���p�X�n
	//-------------------------------------
	//! �g�p���郂�f���̃t�@�C���̃p�X������
	const std::string modelFilePaths[MODELTYPE::TypeMax] =
	{
		"data/model/Player/Player.mv1",
		"data/model/Boss/Boss.mv1",
		"data/model/Stage/Stage.mv1",
		"data/model/Stage/TrainingStage.mv1",
	};

	const std::string fontPath			 = "data/font/phantom.ttf";			//!< �t�H���g�f�[�^
	const std::string fontName			 = "Phantom of the Opera";			//!< �t�H���g��
	const std::string logoImgPath		 = "data/image/TitleLogo.png";		//!< �^�C�g�����S�摜�f�[�^�[
	const std::string targetMakerImgPath = "data/image/TargetMarker.png";	//!< �^�[�Q�b�g�}�[�J�[�f�[�^
	const std::string tutorialTelopPath  = "data/text/TutorialTelops.txt";	//!< �`���[�g���A���̃e���b�v�e�L�X�g

	//! �X�e�[�W�N���X�ɓn���₷���悤�ʕϐ��Ɋi�[
	const std::string stagePaths[2] = { modelFilePaths[MODELTYPE::StageData],
									    modelFilePaths[MODELTYPE::TrainingData] };

	//-------------------------------------
	// �U���̎��Ɏg���ϐ�
	//-------------------------------------
	float hitStop;			//!< �q�b�g�X�g�b�v�{��
	float hitStopTimer;		//!< �q�b�g�X�g�b�v�������Ă鎞��
}

//---------------------------------------------------------------
// �֐�
//---------------------------------------------------------------
// �e�L�����N�^�[�̍U������̔������`�F�b�N����֐�
bool IsAttackSituation(CharacterBase& _attacker)
{
	return _attacker.IsAttack() && _attacker.IsAtkJudge() && !_attacker.IsAtkSend();
}

// �L�����N�^�[���m���Ԃ������Ƃ��ɉ����Ԃ��v�Z
// chara1���y����
void CharaRebound(CharacterBase& chara1, CharacterBase& chara2)
{
	Vector3 vecCenter = (chara1.GetPos() + chara2.GetPos()) / 2.0f;
	Vector3 vecDir = chara2.GetPos() - chara1.GetPos();
	vecDir.y = 0.0f;
	vecDir.SetLength((6.0f + 18.0f) / 2.0f);
	Vector3 newPos = vecCenter - vecDir;
	chara1.SetPos(newPos);
}

// �^�C�g����ʂł̃J�[�\���̈ʒu���玟�̉�ʂ����肳����
void SelectNextScene(const int _cursorValue)
{
	if (_cursorValue == SCENE::Training)
	{
		nowScene = SCENE::Training;
	}

	else if (_cursorValue == SCENE::Movie)
	{
		nowScene = SCENE::Movie;
	}

	if (_cursorValue == SCENE::Edit)
	{
		nowScene = SCENE::Edit;
		// �G�f�B�^�[�Ƀ{�X�N���X�̏���n��
		editor.Init();
	}
}

bool CheckBoxRange(Vector2 _modeBoxPos, VectorInt2 _mousePos, VectorInt2 _size)
{
	if (_mousePos.x <= _modeBoxPos.x)
		return false;

	if (_mousePos.x >= _modeBoxPos.x + _size.x)
		return false;

	if (_mousePos.y <= _modeBoxPos.y)
		return false;

	if (_mousePos.y >= _modeBoxPos.y + _size.y)
		return false;

	return true;
}

bool CheckModeStringRange(Vector2 _modePos, VectorInt2 _mousePos)
{

	if (CheckBoxRange(_modePos, _mousePos, { 220, 42 }))
	{
		return true;
	}

	return false;
}

void DrawFillBoxA(BoxInt _pos, int _alphaValue, unsigned int _colorValue)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alphaValue);
	DrawFillBox(_pos.xTop, _pos.yTop, _pos.xTop + _pos.xUnder, _pos.yTop + _pos.yUnder, _colorValue);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void DrawStringA(VectorInt2 _pos, const TCHAR* _string, int _alphaValue, unsigned int _colorValue)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alphaValue);
	DrawString(_pos.x, _pos.y, _string, _colorValue);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void TitleTelopUpdate()
{
	switch (titleTelopMode)
	{
		// -------------------------------------------
		// �G���^�[�L�[���͑҂����
		// -------------------------------------------
	case TITLETELOPTYPE::PushButton:
		// ���[�h�I��҂���Ԃ�
		if (PushHitKey(KEY_INPUT_RETURN) || PushMouseInput(MOUSE_INPUT_LEFT))
		{
			titleTelopMode = TITLETELOPTYPE::SelectMode;
		}

		break;

		// -------------------------------------------
		// ���[�h�I��҂����
		// -------------------------------------------
	case TITLETELOPTYPE::SelectMode:
		// �㉺�L�[�őI���J�[�\���̈ړ�
		if (PushHitKey(KEY_INPUT_DOWN) || PushHitKey(KEY_INPUT_UP))
		{
			if (selectCursor == SCENE::Training)
			{
				selectCursor = SCENE::Edit;
			}
			else
			{
				selectCursor = SCENE::Training;
			}
		}

		if (PushHitKey(KEY_INPUT_RETURN))
		{
			if (selectCursor == SCENE::Training)
			{
				SetFadeOut();
				//titleTelopMode = TITLETELOPTYPE::TutorialSwitch;
			}
			// �G�f�B�b�g���[�h�֑J��
			else if (selectCursor == SCENE::Edit)
			{
				SetFadeOut();
			}
		}

		break;

		// -------------------------------------------
		// �`���[�g���A���̗L���I��҂����
		// -------------------------------------------
	case TITLETELOPTYPE::TutorialSwitch:
		// �㉺�L�[�őI���J�[�\���̈ړ�
		if (PushHitKey(KEY_INPUT_DOWN) || PushHitKey(KEY_INPUT_UP))
		{
			isTutorial = !isTutorial;
		}

		if (PushHitKey(KEY_INPUT_RETURN))
		{
			SetFadeOut();
		}

		if (PushHitKey(KEY_INPUT_ESCAPE))
		{
			titleTelopMode = TITLETELOPTYPE::SelectMode;
		}

		break;

	default:
		break;
	}
}

void TitleTelopDraw()
{
	int cursorPosY;

	switch (titleTelopMode)
	{
	case TITLETELOPTYPE::PushButton:

		DrawStringA({ 425,(WINDOW_H / 2) + 90 }, "Push Enter Key", alphaValue, GetColor(255, 255, 255));

		break;

	case TITLETELOPTYPE::SelectMode:
		// ���[�h�I���J�[�\���̈ړ�����
		cursorPosY = (selectCursor == SCENE::Training ? (WINDOW_H / 2) + 142 : (WINDOW_H / 2) + 228);

		// �J�[�\���p�{�b�N�X�̕`��
		DrawFillBoxA({ 445,365, cursorPosY,62 }, alphaValue,GetColor(255,255,255));

		// ���[�h�̕�����
		DrawFormatString(448, (WINDOW_H / 2) + 150, GetColor(255, 255, 255), "Game Mode");
		DrawFormatString((WINDOW_W / 2) - 160, (WINDOW_H / 2) + 230, GetColor(255, 255, 255), "Edit Mode");

		break;

	case TITLETELOPTYPE::TutorialSwitch:
		cursorPosY = (isTutorial ? (WINDOW_H / 2) + 142 : (WINDOW_H / 2) + 228);

		// �I���J�[�\���̓����x�ƍ��W�ړ��̏���
		DrawFillBoxA({ 445,365,cursorPosY,cursorPosY + 62 }, alphaValue,GetColor(255,255,255));

		DrawFormatString(448, (WINDOW_H / 2) + 150, GetColor(255, 255, 255), "Tutorial ON");
		DrawFormatString(448, (WINDOW_H / 2) + 230, GetColor(255, 255, 255), "Tutorial OFF");

		break;
	}
}

void FadeUpdate()
{
	FadeInUpdate();
	FadeOutUpdate();
}

void FadeInUpdate()
{
	if (!IsFadeIn()) 
		return;

	screenFadeValue -= 5;
	if (IsfinishFadeIn())
	{
		screenFadeValue = 0;
		isFadeIn = false;
	}
}

void FadeOutUpdate()
{
	if (!IsFadeOut())
		return;

	screenFadeValue += 5;
	if (IsFinishFadeOut())
	{
		screenFadeValue = 255;
		isFadeOut = false;
	}
}

void SetFadeOut()
{
	if (IsFadeIn()) return;
	isFadeOut = true;
}

void ReleaseFadeOut()
{
	isFadeOut = false;
}

void SetFadeIn()
{
	if (IsFadeOut()) return;
	isFadeIn = true;
}

void ReleaseFadeIn()
{
	isFadeIn = false;
}

bool IsFadeOut()
{
	return isFadeOut;
}

bool IsFadeIn()
{
	return isFadeIn;
}

bool IsFinishFadeOut()
{
	if (screenFadeValue >= 255)
	{
		return true;
	}

	return false;
}

bool IsfinishFadeIn()
{
	if (screenFadeValue <= 0)
	{
		return true;
	}

	return false;
}

// �v���C���[�̍U���s���̏���
void PlayerAttack()
{
	// ���G���Ԓ��͉������Ȃ�
	if (boss.IsInvincible())
	{
		return;
	}

	// �f�B�t���[�Y�J���[��Ԃ��_�ł�����
	boss.SetDamage();
	// �_���[�W����
	boss.SetDamage(player.GetPower());
	player.SetAtkSend();
	player.SetEffect();
	hitStopTimer = 10.0f;
	hitStop = 0.0f;

	// �v���C���[�̍U�����R���{�̍ŏI�i�ł���΃{�X�𐁂���΂�
	if (player.IsLastComboAttack())
	{
		boss.SetStateBlowOff();
		return;
	}

	// �{�X���h�䒆�ł���΍U�����e�����
	if (boss.IsGuard())
	{
		player.SetStateRepell();
		boss.SetGuardToAttack();
		return;
	}

}

void BossAttack()
{
	if (player.IsInvincible())
	{
		return;
	}

	player.SetDamage(boss.GetPower());
	player.SetStateBlowOff();
	player.SetEffect();
	boss.SetAtkSend();
}

//---------------------------------------------------------------
// �Q�[������֐�
//---------------------------------------------------------------
// ������
void GameInit()
{

	nowScene = SCENE::Title;
	lockOnMakerImg = LoadGraph(targetMakerImgPath.c_str());
	isDebug = false;
	titleLogoImg = LoadGraph(logoImgPath.c_str());
	AddFontResourceA(fontPath.c_str());
	ChangeFont(fontName.c_str());

	// �e�I�u�W�F�N�g�̏�����
	player.Init(modelFilePaths[MODELTYPE::PlayerData], STAGERADIUS);
	gameStage.Init(stagePaths);
	boss.Init(modelFilePaths[MODELTYPE::BossData]);

	// �e�X�̃N���X�Ŏg���ʃN���X�̏���n��
	player.SetTargetInfo(&boss);
	boss.SetTargetInfo(&player);
	gameCamera.SetPlayerInfo(&player);
	gameCamera.SetTargetInfo(&boss);

	// �J�����̏�����
	gameCamera.Init();

	// �G�f�B�b�g���[�h�Ŏg�p�������n��
	editor.SetPlayerInfo(&player);
	editor.SetBossInfo(&boss);

	tutorial.Init(tutorialTelopPath);

	hitStop      = 1.0f;
	hitStopTimer = 0.0f;

	screenFadeValue	   = 0;
	alphaValue		   = 255;
	titleTelopMode	   = TITLETELOPTYPE::PushButton;
	selectCursor	   = SCENE::Training;
	isTutorial		   = true;
	isViewingMode	   = false;
	isRisingAlphaValue = false;

	// ���[�h�I���̕�������}�E�X�ł������悤�ɂ��Ă�
	gameModeRange.x = (WINDOW_W / 2) - 120;
	gameModeRange.y = (WINDOW_H / 2) + 148;
	editModeRange.x = (WINDOW_W / 2) - 120;
	editModeRange.y = (WINDOW_H / 2) + 198;

}

// �Q�[��������1�x�v���C���邽�߂ɕK�v�Ȃ��̂������ď�����
void ReInit()
{
	nowScene       = SCENE::Title;
	titleTelopMode = TITLETELOPTYPE::PushButton;
	selectCursor   = SCENE::Training;
	isTutorial	   = true;

	gameStage.Init(stagePaths);
	player.ReInit();
	boss.ReInit();
	gameCamera.Init();

	hitStop		 = 1.0f;
	hitStopTimer = 0.0f;
}

// �X�V
void GameUpdate()
{	
	// �V�[�����Ƃ̍X�V����
	switch (nowScene)
	{
		// ----------------------------------------------------
		// �^�C�g���V�[��
		// ----------------------------------------------------
	case SCENE::Title:
		// �uPush Enter Key�v������̓_�ŏ���
		if (isRisingAlphaValue)
		{
			alphaValue += ALPHAHMOVESPEED;
			if (alphaValue >= 255)
			{
				isRisingAlphaValue = !isRisingAlphaValue;
			}
		}
		else
		{
			alphaValue -= ALPHAHMOVESPEED;
			if (alphaValue < 0)
			{
				isRisingAlphaValue = !isRisingAlphaValue;
			}
		}

		// �L�[���͂ŕ\������e���b�v�̃��[�h�ؑ�
		TitleTelopUpdate();

		// �t�F�[�h�l�̍X�V
		FadeUpdate();

		// ��ʂ��t�F�[�h�A�E�g������I��������ʂ�
		if (IsFinishFadeOut())
		{
			isFadeIn = true;
			SelectNextScene(selectCursor);
		}

		tutorial.Update();

		break;

		// ----------------------------------------------------
		// �G�f�B�b�g�V�[��
		// ----------------------------------------------------
	case SCENE::Edit:

		// ��ʈړ����̃t�F�[�h����
		FadeUpdate();

		if (IsFinishFadeOut())
		{
			isFadeOut = false;
			isFadeIn = true;
			ReInit();
		}

		// �X�V
		editor.Update();

		// �t�F�[�h�A�E�g�X�C�b�`���Z�b�g
		if (PushHitKey(KEY_INPUT_ESCAPE))
		{
			if (screenFadeValue <= 0)
			{
				isFadeOut = true;
			}

		}

		break;

		// ----------------------------------------------------
		// �g���[�j���O�V�[��
		// ----------------------------------------------------
	case SCENE::Training:

		FadeInUpdate();

		gameCamera.Update(isDebug);

		// �J�����̐�����������i�s���������߂�
		player.CopyCameraHAngle(gameCamera.GetAngleHorizontal());
		player.Update(hitStop);

		// ���[�v�]�[���ƃv���C���[�̓����蔻��
		if (CheckWarpZoneHit(player.GetPos(), gameStage.GetEffectWarpPos(), gameStage.GetWarpRange()))
		{
			player.ReInit();
			gameCamera.MovieInit();
			nowScene = SCENE::Movie;
		}

		break;

		// ----------------------------------------------------
		// ���[�r�[�V�[��
		// ----------------------------------------------------
	case SCENE::Movie:
		if (isFadeIn)
		{
			screenFadeValue -= 5;
			if (screenFadeValue <= 0)
			{
				screenFadeValue = 0;
				isFadeIn = false;
			}
		}
	
		gameCamera.MovieUpdate();
		boss.MovieUpdate();

		// �{�X�����n���Ă�����J������h�炷
		if (boss.IsGroundMovie())
		{
			gameCamera.SetMovieShakeTime();
		}

		// �J�������̃��[�r�[�����o���I��������S�Ẵ��[�r�[�����o���I���������ƂɂȂ�
		if (gameCamera.IsFinishMovie())
		{
			boss.SetFinishMovie();
			nowScene = SCENE::Game;
		}

		break;

		// ----------------------------------------------------
		// �Q�[���V�[��
		// ----------------------------------------------------
	case SCENE::Game:

		// F1�AF2�L�[�Ńf�o�b�O���[�h�̐؂�ւ�
		if (PushHitKey(KEY_INPUT_F1)) isDebug = !isDebug;

		// --------------------------------------
		// �J�����̏���
		// --------------------------------------
		// �J�����̐����p�x���v���C���[�N���X�ɃR�s�[����
		player.CopyCameraHAngle(gameCamera.GetAngleHorizontal());
		// �X�V����
		gameCamera.Update(isDebug);

		// --------------------------------------
		// �q�b�g�X�g�b�v�̃^�C�}�[�̏���
		// --------------------------------------
		if (hitStop < 1.0f)	hitStopTimer--;

		if (hitStopTimer < 0)
		{
			hitStopTimer = 0.0f;
			hitStop = 1.0f;
		}

		// --------------------------------------
		// �v���C���[�֘A�̏���
		// --------------------------------------
		player.Update(hitStop);

		// �s�k����
		if (player.IsDeath()) nowScene = SCENE::Finish;

		// --------------------------------------
		// �{�X�֘A�̏���
		// --------------------------------------
		boss.Update(hitStop);

		// ���S����
		if (boss.IsDeath()) nowScene = SCENE::Finish;

		// --------------------------------------
		// �����蔻��̏���
		// --------------------------------------
		// �L�����N�^�[���m�̂��蔲���h�~
		if (CheckBallHit(player.GetPos(), 6.0f, boss.GetPos(), 18.0f))
		{
			CharaRebound(player, boss);
			// �{�X���U����Ԃ�
			if (boss.IsSeePlayer() && !boss.IsGuard())
			{
				boss.ChoiseAttackType();
			}
		} 

		// ---------------------------------
		// �v���C���[����{�X�ւ̓����蔻��
		// ---------------------------------
		if (CheckAttackHit(player, boss, isDebug))
		{
			if (IsAttackSituation(player))
			{
				PlayerAttack();
			}
		}

		// ----------------------------------
		// �{�X����v���C���[�ւ̓����蔻��
		// ---------------------------------
		// �E�r�ł̍U��
		if (CheckBossFistAttack(boss, player, isDebug))
		{
			if (IsAttackSituation(boss))
			{
				BossAttack();
			}
		}

		// ���r�ł̍U��
		if (CheckBossClawAttack(boss, player, isDebug))
		{
			if (IsAttackSituation(boss))
			{
				BossAttack();
			}
		}

		// ���r�ł̍U��
		if (CheckBossComboAttack(boss, player, isDebug))
		{
			if (IsAttackSituation(boss))
			{
				BossAttack();
			}
		}

		break;

		// ----------------------------------------------------
		// �Q�[���I���V�[��
		// ----------------------------------------------------
	case SCENE::Finish:

		// �L�[���͂Ŋe�l�����������ă^�C�g���֖߂�
		if (PushHitKey(KEY_INPUT_RETURN) || PushMouseInput(MOUSE_INPUT_LEFT))
		{
			SetFadeOut();
		}

		FadeOutUpdate();
		
		if (screenFadeValue >= 255)
		{
			isFadeOut = false;
			isFadeIn = true;
			ReInit();
		}
		break;
	}

}

// �`��
void GameDraw()
{
	// ---------------------------------------
	// �V���h�E�}�b�v�ւ̕`��
	// ---------------------------------------
	// �`�揀��
	ShadowMap_DrawSetup(handleHandle());

	// ���ꂼ��̃��f�����V���h�E�}�b�v�֕`��
	gameStage.StageDraw();
	player.Draw();
	boss.Draw();

	// �V���h�E�}�b�v�ւ̕`����I��
	ShadowMap_DrawEnd();

	// ---------------------------------------
	// switch���Ŏg�p����ϐ��̐錾
	// ---------------------------------------
	const int LOGOFONTSIZE	  = 84;		//!< �^�C�g�����S�p�̃T�C�Y
	const int MODEFONTSIZE	  = 42;		//!< �^�C�g�����S�p�̃T�C�Y
	const int DEFAULTFONTSIZE = 14;		//!< DX���C�u�����̏����T�C�Y

	gameCamera.Draw();
	if (nowScene != Edit && nowScene != Training)
	{
		gameStage.StageDraw();
	}

	//Training�X�e�[�W���`��ł���悤�ɂȂ�������ATraining�@�����@�Q�[���X�e�[�W�ɍs����悤�ɂ���


	// �V�[�����Ƃ̕`�揈��
	switch (nowScene)
	{
		// ----------------------------------------------------
		// �^�C�g���V�[��
		// ----------------------------------------------------
	case SCENE::Title:
		SetFontSize(62);

		TitleTelopDraw();

		SetFontSize(DEFAULTFONTSIZE);

		if (isDebug)
		{
			DrawLine(WINDOW_W / 2, 0, WINDOW_W / 2, WINDOW_H, GetColor(0, 0, 255));
			DrawLine(0, WINDOW_H / 2, WINDOW_W, WINDOW_H / 2, GetColor(255, 0, 0));
		}

		DrawGraph((WINDOW_W / 2) - 450, (WINDOW_H / 2) - 140, titleLogoImg, TRUE);

		break;

		// ----------------------------------------------------
		// �G�f�B�b�g�V�[��
		// ----------------------------------------------------
	case SCENE::Edit:
		editor.Draw();

		break;

		// ----------------------------------------------------
		// �g���[�j���O�V�[��
		// ----------------------------------------------------
	case SCENE::Training:
		gameStage.TrainingDraw();

		player.Draw();
		player.PostDraw();

		break;

		// ----------------------------------------------------
		// ���[�r�[�V�[��
		// �Q�[���V�[��
		// ----------------------------------------------------
	case SCENE::Movie:
	case SCENE::Game:
		// �`��Ɏg�p����V���h�E�}�b�v��ݒ�
		SetUseShadowMap(0, handleHandle());

		boss.Draw();
		boss.DamageDraw();
		player.Draw();

		if (player.IsTargetLook())
		{
			// �}�[�J�[�̕`��
			Vector3 makerPos = boss.GetPos();
			makerPos.y += 30.0f;
			DrawGraph3D(makerPos.x, makerPos.y, makerPos.z, lockOnMakerImg, TRUE);
		}

		// �V���h�E�}�b�v�̐ݒ������
		SetUseShadowMap(0, -1);

		// �X�e�[�W�̕`��
		DrawSphere3D(VGet(0.0f, 0.0, 0.0f), STAGERADIUS, 32, GetColor(128, 24, 24), GetColor(128, 24, 24),FALSE);

		// ��ԑO�ɕ`�悵�������́iUI�Ȃǁj
		player.PostDraw();
		boss.PostDraw();

		break;

		// ----------------------------------------------------
		// �Q�[���I���V�[��
		// ----------------------------------------------------
	case SCENE::Finish:

		SetFontSize(LOGOFONTSIZE);
		// ���s�ɂ���ĕ`�悷�镶�����ς���
		if (player.IsDeath())
		{
			DrawFormatString((WINDOW_W / 2) - 200, (WINDOW_H / 2) - 100, GetColor(16, 16, 255), " L O S E ");
		}
		if (boss.IsDeath())
		{
			DrawFormatString((WINDOW_W / 2) - 150, (WINDOW_H / 2) - 100, GetColor(255, 16, 16), " W I N ");
		}
		SetFontSize(DEFAULTFONTSIZE);
		DrawFormatString((WINDOW_W / 2) - 52, (WINDOW_H / 2) + 70, GetColor(255, 255, 255), "Push Any Key");

		break;
	}

}

// �őO�ʂɕ`�悵�������̂�����
void GamePostDraw()
{

	// ���[�r�[�����o���͉�ʂ̏㉺�ɍ�����`�悵��
	// ���[�U�[�Ƀ��[�r�[���ł��邱�Ƃ𕪂���₷������
	if (nowScene == Movie)
	{
		DrawFillBox(0,            0, WINDOW_W,       50, GetColor(0, 0, 0));
		DrawFillBox(0, WINDOW_H - 50, WINDOW_W, WINDOW_H, GetColor(0, 0, 0));
	}

	// ��ʃt�F�[�h�A�E�g�p�̃{�b�N�X
	DrawFillBoxA({ 0,WINDOW_W,0,WINDOW_H }, screenFadeValue, GetColor(0, 0, 0));
}

// �I��
void GameExit()
{
	gameCamera.Exit();
	gameStage.Exit();
	player.Exit();
	boss.Exit();
}