<?hh //strict

namespace sandbox;

type Experience = mixed;
type Skills = Map<string, Vector<Experience>>;

<<__ConsistentConstruct>>
abstract class SoftwareEngineer {

    protected Skills $skills = Map{};

    abstract public async function improveSkill(string $skill):Awaitable<bool>;
    abstract public function haveSomeSkills():bool;

    public function __construct() {
      echo 'Hello, world!';
    }
}

class HackLangDeveloper extends SoftwareEngineer {

    const KNOWLEDGE_SOURCE = 'https://docs.hhvm.com/hack/';

    public async function doLearn():Awaitable<void> {

        $tasks = Vector{};
        $tasks->add($this->improveSkill('hhvm'));
        $tasks->add($this->improveSkill('hacklang'));
        $tasks->add($this->improveSkill('asio'));
        $tasks->add($this->improveSkill('php'));

        await \HH\Asio\v($tasks);
    }

	public async function improveSkill(string $skill):Awaitable<bool> {

        $knowledge = await \HH\Asio\curl_exec(
            static::KNOWLEDGE_SOURCE . '?tag=' . $skill . '&random_article'
        );

        if ($knowledge === '') {
            return false;
        }

        if (!$this->tryApplyKnowledge($knowledge)) {
            await RescheduleWaitHandle::create(
                RescheduleWaitHandle::QUEUE_DEFAULT,
                0,
            );
            return false;
        }

		if (!$this->skills->contains($skill)) {
			$this->skills->add(Pair{$skill, Vector{}});
		}

		$this->skills[$skill]->add($knowledge);
        return true;
	}

    public function tryApplyKnowledge(string $knowledge):bool {
        return rand(1,9999) % 123 === 0;
    }

    public function haveSomeSkills():bool {
        return $this->skills !== Map{};
    }

	public function getSkills():Skills {
		return $this->skills;
	}
}

class Greetings {

    public static function sayCheers(mixed $stranger):void {

        if (!$stranger instanceof HackLangDeveloper) {
            $stranger = new HackLangDeveloper();
        }

        while (!$stranger->haveSomeSkills()) {
            $stranger->doLearn()->getWaitHandle()->join();
        }

        echo 'Happy programmer\'s day';
    }
}
