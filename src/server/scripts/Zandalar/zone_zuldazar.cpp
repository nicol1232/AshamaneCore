/*
 * Copyright (C) 2017-2018 AshamaneProject <https://github.com/AshamaneProject>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Player.h"
#include "ScriptedEscortAI.h"

enum ZuldazarQuests
{
    QUEST_WELCOME_ZULDAZAR      = 46957,
    QUEST_RASTAKHAN             = 46930,

    QUEST_SPEAKER_OF_THE_HORDE  = 46931,
    OBJECTIVE_SUMMON_THE_HORDE  = 121000,

    QUEST_NEED_EACH_OTHER       = 52131,
};

enum ZuldazarSpells
{
    SPELL_TALANJI_EXPOSITION_CONVERSATION_1 = 261541,
    SPELL_TALANJI_EXPOSITION_CONVERSATION_2 = 261549,
    SPELL_TALANJI_EXPOSITION_KILL_CREDIT    = 265711,

    SPELL_RASTAKHAN_GREETINGS_SCENE         = 244534,

    SPELL_PREVIEW_TO_ZANDALAR               = 273387,
};

enum ZuldazarNpcs
{
    NPC_ZOLANI                      = 135441,
    NPC_FOLLOW_ZOLANI_KILL_CREDIT   = 120169,
};

// 132332
struct npc_talanji_arrival : public ScriptedAI
{
    npc_talanji_arrival(Creature* creature) : ScriptedAI(creature) { }

    void sQuestAccept(Player* player, Quest const* /*quest*/) override
    {
        me->DestroyForPlayer(player);
    }
};

// 132661
struct npc_talanji_arrival_escort : public npc_escortAI
{
    npc_talanji_arrival_escort(Creature* creature) : npc_escortAI(creature) { }

    void IsSummonedBy(Unit* summoner) override
    {
        me->Mount(80358);
        Start(false, true, summoner->GetGUID());
        SetDespawnAtEnd(false);
        summoner->CastSpell(summoner, SPELL_TALANJI_EXPOSITION_CONVERSATION_1, true);
    }

    void LastWaypointReached() override
    {
        me->SetFacingTo(0.f);
        me->CastSpell(me, SPELL_TALANJI_EXPOSITION_KILL_CREDIT, true);

        if (Player* player = GetPlayerForEscort())
            player->CastSpell(player, SPELL_TALANJI_EXPOSITION_CONVERSATION_2, true);
    }
};

// 138912
struct npc_enforcer_pterrordax : public npc_escortAI
{
    npc_enforcer_pterrordax(Creature* creature) : npc_escortAI(creature) { }

    void IsSummonedBy(Unit* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
        {
            if (player->GetQuestStatus(QUEST_RASTAKHAN) == QUEST_STATUS_INCOMPLETE)
            {
                KillCreditMe(player);
                me->SetSpeed(MOVE_RUN, 21.f);
                player->EnterVehicle(me);
                Start(false, true, player->GetGUID());
            }
        }
    }

    void LastWaypointReached() override
    {
        if (Player* player = GetPlayerForEscort())
            player->CastSpell(player, SPELL_RASTAKHAN_GREETINGS_SCENE, true);
    }
};

// 120740
struct npc_rastakhan_zuldazar_arrival : public ScriptedAI
{
    npc_rastakhan_zuldazar_arrival(Creature* creature) : ScriptedAI(creature) { }

    void sQuestAccept(Player* player, Quest const* quest) override
    {
        if (quest->ID == QUEST_SPEAKER_OF_THE_HORDE)
            player->SummonCreature(NPC_ZOLANI, -1100.689941f, 817.934021f, 497.243011f, 6.062160f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120000, true);
    }
};

// 135441
struct npc_soth_zolani : public npc_escortAI
{
    npc_soth_zolani(Creature* creature) : npc_escortAI(creature) { }

    void sGossipHello(Player* player) override
    {
        SetDespawnAtEnd(false);
        Start(false, false, player->GetGUID());
        Talk(0);
    }

    void LastWaypointReached() override
    {
        if (Player* player = GetPlayerForEscort())
        {
            Talk(1);
            player->KilledMonsterCredit(NPC_FOLLOW_ZOLANI_KILL_CREDIT);
            me->ForcedDespawn(10000);
        }
    }
};

// 122690
struct npc_brillin_the_beauty : public ScriptedAI
{
    npc_brillin_the_beauty(Creature* creature) : ScriptedAI(creature) { }

    void MoveInLineOfSight(Unit* unit) override
    {
        if (Player* player = unit->ToPlayer())
            if (player->GetDistance(me) < 10.f)
                if (player->GetQuestStatus(QUEST_SPEAKER_OF_THE_HORDE) == QUEST_STATUS_INCOMPLETE)
                    if (player->GetQuestObjectiveCounter(OBJECTIVE_SUMMON_THE_HORDE) != 0)
                        KillCreditMe(player);
    }
};

// 130984
struct npc_natal_hakata : public ScriptedAI
{
    npc_natal_hakata(Creature* creature) : ScriptedAI(creature) { }

    void MoveInLineOfSight(Unit* unit) override
    {
        if (Player* player = unit->ToPlayer())
            if (player->GetDistance(me) < 10.f)
                if (player->GetQuestStatus(QUEST_SPEAKER_OF_THE_HORDE) == QUEST_STATUS_INCOMPLETE)
                    if (player->GetQuestObjectiveCounter(OBJECTIVE_SUMMON_THE_HORDE) != 0)
                        KillCreditMe(player);
    }
};

// 131443
struct npc_telemancer_oculeth_zuldazar : public ScriptedAI
{
    npc_telemancer_oculeth_zuldazar(Creature* creature) : ScriptedAI(creature) { }

    void MoveInLineOfSight(Unit* unit) override
    {
        if (Player* player = unit->ToPlayer())
            if (player->GetDistance(me) < 10.f)
                if (player->GetQuestStatus(QUEST_SPEAKER_OF_THE_HORDE) == QUEST_STATUS_INCOMPLETE)
                    if (player->GetQuestObjectiveCounter(OBJECTIVE_SUMMON_THE_HORDE) != 0)
                        player->KilledMonsterCredit(135435);
    }
};

// 133050
struct npc_talanji_great_seal : public ScriptedAI
{
    npc_talanji_great_seal(Creature* creature) : ScriptedAI(creature) { }

    void sQuestAccept(Player* player, Quest const* quest) override
    {
        if (quest->ID == QUEST_NEED_EACH_OTHER)
            player->CastSpell(player, SPELL_PREVIEW_TO_ZANDALAR, true);
    }
};

void AddSC_zone_zuldazar()
{
    RegisterCreatureAI(npc_talanji_arrival);
    RegisterCreatureAI(npc_talanji_arrival_escort);
    RegisterCreatureAI(npc_enforcer_pterrordax);
    RegisterCreatureAI(npc_rastakhan_zuldazar_arrival);
    RegisterCreatureAI(npc_soth_zolani);
    RegisterCreatureAI(npc_brillin_the_beauty);
    RegisterCreatureAI(npc_natal_hakata);
    RegisterCreatureAI(npc_telemancer_oculeth_zuldazar);
    RegisterCreatureAI(npc_talanji_great_seal);
}