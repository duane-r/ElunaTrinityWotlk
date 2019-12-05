#include "Chat.h"
#include "ScriptMgr.h"
#include "Pet.h"


#define VERBOSE 0


// This assumes that every dungeon is five-man, and may not be
//  terribly efficient. (Not an issue for me.)

class custom_make_instances_easier : public PlayerScript, UnitScript
{
    public:
        custom_make_instances_easier() : PlayerScript("custom_make_instances_easier"), UnitScript("custom_make_instances_easier") { }

        void OnDamage(Unit* attacker, Unit* victim, uint32& damage)
        {
            /*
            if (attacker && victim)
            {
                ObjectGuid ag = attacker->GetGUID();
                if (ag.IsPlayer() && attacker->getClass() == CLASS_WARLOCK)
                {
                    damage *= 3;
                }
            }
            */

            if (!victim || !victim->GetMap()->IsDungeon())
                return;

            uint32 count = victim->GetMap()->GetPlayersCountExceptGMs();
            if (count > 3)
                return;

            float mult = 2.0;
            if (count == 2)
                mult = 1.5;
            if (count == 3)
                mult = 1.25;

            ObjectGuid vg = victim->GetGUID();
            if (vg.IsPlayer() || (vg.IsPet() && (victim->GetOwnerGUID()).IsPlayer()))
            {
                damage /= mult;

                if (VERBOSE && vg.IsPlayer())
                    ChatHandler(((Player *) victim)->GetSession()).PSendSysMessage("You take %d damage. It would have been %d.\n", damage, (int)(damage * mult));

                return;
            }

            if (!attacker)
                return;

            ObjectGuid ag = attacker->GetGUID();
            if (ag.IsPlayer() || (ag.IsPet() && (attacker->GetOwnerGUID()).IsPlayer()))
            {
                damage *= mult;

                if (VERBOSE && ag.IsPlayer())
                    ChatHandler(((Player *) attacker)->GetSession()).PSendSysMessage("You do %d damage. It would have been %d.\n", damage, (int)(damage / mult));

                return;
            }
        }


        void OnGiveXP(Player* player, uint32& amount, Unit* victim)
        {
            if (!player || !player->GetMap()->IsDungeon())
                return;

            uint32 count = player->GetMap()->GetPlayersCountExceptGMs();
            if (count > 3)
                return;

            uint32 mult = 3;

            if (count == 1)
                mult = 5;

            //printf("experience lowered from %d to %d\n", amount, amount / 5);
            amount /= mult;
        }
};

void AddSC_custom_make_instances_easier()
{
    new custom_make_instances_easier();
}
