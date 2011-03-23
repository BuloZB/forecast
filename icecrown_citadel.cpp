
#include "ScriptPCH.h"
#include "icecrown_citadel.h"
#include "Spell.h"

enum Spells
{
    SPELL_ON_ORGRIMS_HAMMER_DECK   = 70121, 
    SPELL_DARKMARTYRDOM_FANATIC     = 71236,
    SPELL_DARKMARTYRDOM_ADHERENT    = 70903,
    SPELL_DARKTRANSFORMATION        = 70900,
    SPELL_NECROTICSTRIKE            = 70659,
    SPELL_SHADOWCLEAVE              = 70670,
    SPELL_VAMPIRICMIGHT             = 70674,
    SPELL_FANATIC_DETERMINATION     = 71235,
    SPELL_ADHERENT_DETERMINATION    = 71234,
    SPELL_EMPOWEREMENT              = 70901,
    SPELL_FROST_FEVER               = 67767,
    SPELL_DEATHCHILL_BLAST          = 70906,
    SPELL_DEATHCHILL_BOLT           = 70594,
    SPELL_CURSE_OF_TORPOR           = 71237,
    SPELL_SHORUD_OF_THE_OCCULT      = 70768,
    SPELL_DARK_TRANSFORMATION_T     = 70895,
    SPELL_DARK_EMPOWERMENT_T        = 70896,
    SPELL_STANDART_HORDE            = 69811
};

enum TeleportSpells
{
    HAMMER        = 70781,
    ORATORY       = 70856,
    RAMPART       = 70857,
    SAURFANG      = 70858,
    UPPER_SPIRE   = 70859,
    PLAGUEWORKS   = 9995,
    CRIMSONHALL   = 9996,
    FWHALLS       = 9997,
    QUEEN         = 70861,
    LICHKING      = 70860
};

class npc_cult_fanatic_and_adherent : public CreatureScript
{
    public:
        npc_cult_fanatic_and_adherent() : CreatureScript("npc_cult_fanatic_and_adherent") { }

        struct npc_cult_fanatic_and_adherentAI : public ScriptedAI
        {
            npc_cult_fanatic_and_adherentAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                uiStrikeTimer = urand(8000, 11000);
                uiVampirTimer = urand(19000, 22000);
                uiCleaveTimer = urand(7000, 9000);
                uiMartyrdomTimer = urand(24000, 30000);
                uiFrostFeverTimer = urand(9000, 11000);
                uiDeathchillTimer = urand(10000, 12000);
                uiCurseTimer = urand(8000, 10000);
                uiOccultTimer = urand(25000, 29000);

                bEmpowerement = false;

                if(me->GetEntry() == CREATURE_REANIMATED_ADHERENT)
                    DoCast(me, SPELL_ADHERENT_DETERMINATION);
                if(me->GetEntry() == CREATURE_REANIMATED_FANATIC)
                    DoCast(me, SPELL_FANATIC_DETERMINATION);
            }

            void JustDied(Unit* /*who*/) { }

            void SpellHit(Unit* /*caster*/, const SpellEntry * spell)
            {
                if(me->GetEntry() == CREATURE_ADHERENT)
                {
                    if (spell->Id == SPELL_EMPOWEREMENT)
                        me->UpdateEntry(CREATURE_EMPOWERED_ADHERENT);
                    else if (spell->Id == SPELL_DARK_EMPOWERMENT_T)
                    {
                        DoCast(me, SPELL_EMPOWEREMENT);
                        bEmpowerement = true;
                    }
                }
                if(me->GetEntry() == CREATURE_FANATIC)
                {
                    if (spell->Id == SPELL_DARKTRANSFORMATION)
                        me->UpdateEntry(CREATURE_DEFORMED_FANATIC);
                    else if (spell->Id == SPELL_DARK_TRANSFORMATION_T)
                        DoCast(me, SPELL_DARKTRANSFORMATION);
                }
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STAT_CASTING))
                    return;

                if((me->GetEntry() == CREATURE_ADHERENT) || (me->GetEntry() == CREATURE_EMPOWERED_ADHERENT) || (me->GetEntry() == CREATURE_REANIMATED_ADHERENT))
                {
                    if (uiMartyrdomTimer <= uiDiff && !bEmpowerement)
                    {
                        DoCast(me, SPELL_DARKMARTYRDOM_ADHERENT);
                        uiMartyrdomTimer = urand(24000, 30000);
                    } else uiMartyrdomTimer -= uiDiff;

                    if (uiFrostFeverTimer <= uiDiff)
                    {
                        DoCast(me->getVictim(), SPELL_FROST_FEVER);
                        uiFrostFeverTimer = urand(9000, 11000);
                    } else uiFrostFeverTimer -= uiDiff;

                    if (uiDeathchillTimer <= uiDiff)
                    {
                        if (me->GetEntry() == CREATURE_EMPOWERED_ADHERENT)
                            DoCast(me->getVictim(), SPELL_DEATHCHILL_BLAST);
                        else
                            DoCast(me->getVictim(), SPELL_DEATHCHILL_BOLT);
                        uiDeathchillTimer = urand(10000, 12000);
                    } else uiDeathchillTimer -= uiDiff;

                    if (uiCurseTimer <= uiDiff)
                    {
                        if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 1))
                            DoCast(target, SPELL_CURSE_OF_TORPOR);
                        uiCurseTimer = urand(8000, 10000);
                    } else uiCurseTimer -= uiDiff;

                    if (uiOccultTimer <= uiDiff)
                    {
                        DoCast(me, SPELL_SHORUD_OF_THE_OCCULT);
                        uiOccultTimer = urand(25000, 29000);
                    } else uiOccultTimer -= uiDiff;
                }
                if((me->GetEntry() == CREATURE_FANATIC) || (me->GetEntry() == CREATURE_REANIMATED_FANATIC) || (me->GetEntry() == CREATURE_DEFORMED_FANATIC))
                {
                    if (uiMartyrdomTimer <= uiDiff)
                    {
                        DoCast(me, SPELL_DARKMARTYRDOM_FANATIC);
                        uiMartyrdomTimer = urand(24000, 30000);
                    } else uiMartyrdomTimer -= uiDiff;

                    if (uiStrikeTimer <= uiDiff)
                    {
                        DoCast(me, SPELL_NECROTICSTRIKE);
                        uiStrikeTimer = urand(8000, 11000);
                    } else uiStrikeTimer -= uiDiff;

                    if (uiCleaveTimer <= uiDiff)
                    {
                        DoCast(me, SPELL_SHADOWCLEAVE);
                        uiCleaveTimer = urand(7000, 9000);
                    } else uiCleaveTimer -= uiDiff;

                    if (uiVampirTimer <= uiDiff)
                    {
                        DoCast(me, SPELL_VAMPIRICMIGHT);
                        uiVampirTimer = urand(19000, 22000);
                    } else uiVampirTimer -= uiDiff;
                }

                DoMeleeAttackIfReady();
            }

        private:
            uint32 uiStrikeTimer;
            uint32 uiCleaveTimer;
            uint32 uiVampirTimer;
            uint32 uiMartyrdomTimer;
            uint32 uiFrostFeverTimer;
            uint32 uiDeathchillTimer;
            uint32 uiCurseTimer;
            uint32 uiOccultTimer;

            bool bEmpowerement;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_cult_fanatic_and_adherentAI(creature);
        }
};

class go_icecrown_teleporter : public GameObjectScript
{
    public:
        go_icecrown_teleporter() : GameObjectScript("go_icecrown_teleporter") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
            InstanceScript* instance = go->GetInstanceScript();
            if(!instance)
                return false;

            if(instance->GetData(DATA_MARROWGAR_EVENT) == DONE)
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to Light's Hammer.", GOSSIP_SENDER_MAIN, HAMMER);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to the Oratory of the Damned.", GOSSIP_SENDER_MAIN, ORATORY);
            }
            if(instance->GetData(DATA_DEATHWHISPER_EVENT) == DONE)
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to the Rampart of Skulls.", GOSSIP_SENDER_MAIN, RAMPART);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to the Deathbringer's Rise.", GOSSIP_SENDER_MAIN, SAURFANG);
            }
            if(instance->GetData(DATA_SAURFANG_EVENT) == DONE)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to the Upper Spire.", GOSSIP_SENDER_MAIN, UPPER_SPIRE);
            if(instance->GetData(DATA_PROFESSOR_PUTRICIDE_EVENT) == DONE)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to the Plagueworks", GOSSIP_SENDER_MAIN, PLAGUEWORKS);
            if(instance->GetData(DATA_BLOOD_QUEEN_LANATHEL_EVENT) == DONE)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to the Crimson Halls", GOSSIP_SENDER_MAIN, CRIMSONHALL);
            if(instance->GetData(DATA_VALITHRIA_DREAMWALKER_EVENT) == DONE)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to the Sindragosa's Lair", GOSSIP_SENDER_MAIN, QUEEN);
            if(instance->GetData(DATA_SINDRAGOSA_EVENT) == DONE)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to the Frostwing Halls", GOSSIP_SENDER_MAIN, FWHALLS);

            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, GameObject* /*go*/, uint32 uiSender, uint32 uiAction)
        {
            //player->PlayerTalkClass->ClearMenus();
            if(!player->getAttackers().empty())
                return false;

            SpellEntry const* spell = sSpellStore.LookupEntry(uiAction);
            if (!spell)
                return false;

            if (player->isInCombat())
            {
                Spell::SendCastResult(player, spell, 0, SPELL_FAILED_AFFECTING_COMBAT);
                return true;
            }

            if (uiSender == GOSSIP_SENDER_MAIN)
                player->CastSpell(player, spell, true);

            return true;
        }
};

void AddSC_icecrown_citadel()
{
    new npc_cult_fanatic_and_adherent();
    new go_icecrown_teleporter();
}
